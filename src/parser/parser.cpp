#include "parser/parser.hpp"

#include <ctype.h>
#include <assert.h>

namespace memdb 
{

    // SQLCommand* Parser::parse() {

    // }

    static const std::unordered_map<std::string, CommandType>
        str_to_command_mp {
            {"CREATE TABLE", CreateTable},
            {"INSERT",     Insert},
            {"UPDATE",     Update},
            {"SELECT",     Select},
            {"DELETE",     Delete},
            {"JOIN",     Join},
            {"CREATE INDEX", CreateIndex}
        };

    static const std::unordered_map<std::string, KeywordType>
        str_to_keyword_mp {
            {"TO",         To},
            {"FROM",     From},
            {"WHERE",     Where},
            {"SET",     Set},
            {"ON",         On},
            {"INDEX ON", IndexOn},
            {"BY",         By}
        };

    static const std::unordered_map<std::string, ColumnAttribute>
        str_to_attr_mp {
            {"key", Key},
            {"unique", Unique},
            {"autoincrement", Autoincrement}
        };

    static const std::unordered_map<std::string, Operation> 
        str_to_op = {
            { "+" , ADD },
            { "-" , SUB },
            { "*" , MUL },
            { "/" , DIV },
            { "%" , MOD },
            { "||", OR  },
            { "&&", AND },
            { "!" , NOT },
            { "^" , XOR },
            {  "&", BAND},
            { "|" , BOR },
            {  "~", BNEG},
            { "==", EQ  },
            { "!=", NEQ },
            { "<" , LE  },
            { "<=", LEQ },
             { ">" , GR  },
            { ">=", GEQ } 
        };

    static const std::unordered_map<std::string, size_t>
        str_to_prior = {
            { "||", 0 },
            { "&&", 1 },
            { "|" , 2 },
            { "^" , 3 },
            { "&" , 4 },
            { "==", 5 },
            { "!=", 5 },
            { "<" , 6 },
            { "<=", 6 },
             { ">" , 6 },
            { ">=", 6 },
            { "+" , 7 },
            { "-" , 7 },
            { "*" , 8 },
            { "/" , 8 },
            { "%" , 8 },
            { "!" , 9 },
            {  "~", 9 }
        };

    bool token_is_operation(const std::string& token)
    {
        return (str_to_op.find(token) != str_to_op.end());
    }

    unsigned char char_to_hex(char a)
    {
        if (!isxdigit(a)) 
            throw InvalidHexValueException();
        if (isdigit(a))
            return a - '0';
        return tolower(a) - 'a' + 10;
    }

    unsigned char two_chars_to_hex(char a, char b)
    {
        return char_to_hex(a) * 16 + char_to_hex(b);    
    }

    // convert escape sequences to characters
    std::string unescape(const std::string& str)
    {
        std::string res = "";
        for (auto i = 0LU; i < str.size(); i++)
        {
            if (str[i] != '\\') {
                res += str[i]; // append normal characters
                continue;
            }

            if (i + 1 == str.size())
                throw InvalidEscapeSequenceException();

            switch(str[++i]) 
            {
            case 'a': res += '\a'; break;
            case 'b': res += '\b'; break;
            case 'f': res += '\f'; break;
            case 'n': res += '\n'; break;
            case 'r': res += '\r'; break;
            case 't': res += '\t'; break;
            case 'v': res += '\v'; break;
            case '\'': res += '\''; break;
            case '\"': res += '\"'; break;
            case '\\': res += '\\'; break;
            case '\?': res += '\?'; break;
            case 'x':
                char hex = 0;
                if (i + 1 == str.size()) throw InvalidEscapeSequenceException();
                if (isxdigit(str[++i])) 
                    hex += str[i];

                if (i + 1 == str.size()) throw InvalidEscapeSequenceException();
                if (isxdigit(str[++i])) { 
                    hex *= 16; 
                    hex += str[i]; 
                }
                res += hex;
                break;  
            }
        }
        return res;
    }

    CommandType str_to_command(std::string& str)
    {
        for (auto i = 0LU; i < str.size(); ++i)
            if (isalpha(str[i]))
                str[i] = toupper(str[i]); // convert all letters to upppercase

        assert(str_to_command_mp.find(str) != str_to_command_mp.end());
        return str_to_command_mp.at(str); // return command type from map
    }

    KeywordType str_to_keyword(std::string& str)
    {
        for (auto i = 0LU; i < str.size(); ++i)
            if (isalpha(str[i]))
                str[i] = toupper(str[i]); // convert all letters to upppercase

        assert(str_to_keyword_mp.find(str) != str_to_keyword_mp.end());
        return str_to_keyword_mp.at(str); // return command type from map
    }

    ColumnAttribute str_to_attr(std::string& str)
    {
        assert(str_to_attr_mp.find(str) != str_to_attr_mp.end());
        return str_to_attr_mp.at(str); // return attribute from map
    }

    Parser::Parser(std::string&& query)
    : query_(query), pos_(), end_()
    { 
        pos_ = query_.begin();
        end_ = query_.end();
    }

    bool Parser::parse_pattern(std::regex regexp)
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status) {
            pos_ = match_result[0].second;
        }

        return match_status;
    }

    bool Parser::parse_pattern(std::regex regexp, std::string& ret) 
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status) {
            pos_ = match_result[0].second;
            ret = std::string(match_result[0].first, match_result[0].second);
        }

        return match_status;
    }

    bool Parser::parse_pattern_static(std::regex regexp, Position& pos_, Position& end_) 
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status)
            pos_ = match_result[0].second;


        return match_status;
    }

    bool Parser::parse_pattern_static(std::regex regexp, Position& pos_, Position& end_, std::string& ret) 
    {
        std::smatch match_result{}; // Match regex in std::string

        bool match_status = std::regex_search(
            pos_, end_,                              // The iterators limiting the matched sequence
            match_result,                            // Match result
            regexp,                                  // Regular expression specifying the pattern
            std::regex_constants::match_continuous); // Start matching from the beginning (pos_)

        // Move the iterator on success:
        if (match_status) {
            pos_ = match_result[0].second;
            ret = std::string(match_result[0].first, match_result[0].second);
        }

        return match_status;
    }

    bool Parser::parse_whitespaces()
    {
        static const std::regex pattern{"\\s*"};
        return parse_pattern(pattern);
    }

    bool Parser::parse_comma()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{","};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_colon()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{":"};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_equal_sign()
    {
        parse_whitespaces(); // skip whitespaces before comma

        static const std::regex pattern{"="};
        bool res = parse_pattern(pattern); // parse comma

        parse_whitespaces(); // skip whitespaces after comma
        return res;
    }

    bool Parser::parse_command(CommandType& ret)
    {
        static const std::regex 
            pattern{"(?i)(CREATE\\s+TABLE)|(INSERT)|(SELECT)|(UPDATE)|(DELETE)"};
        // Note: (?i) is a flag that makes pattern  case insensitive

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str_to_command(str);     // assign ret to matched pattern
        return res;
    }

    bool Parser::parse_keyword(KeywordType& ret)
    {
        static const std::regex 
            pattern{"(?i)(TO)|(FROM)|(WHERE)|(SET)|(ON)|(INDEX\\s+ON)|(BY)"};
        // Note: (?i) is a flag that makes pattern  case insensitive

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str_to_keyword(str); // assign ret to matched pattern
        return res;
    }

    bool Parser::parse_name(std::string& ret)
    {
        static const std::regex 
            pattern{"[A-Za-z0-9_\\-]+"};
        return parse_pattern(pattern, ret);
    }

    bool Parser::parse_subquery(std::string& ret) 
    {
        static const std::regex 
            pattern{"\\(.+\\)"};
        return parse_pattern(pattern, ret);
    }

    bool Parser::parse_int(int& ret)
    {
        static const std::regex
            pattern("([1-9][0-9]*)|([0-9])");

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = std::stoi(str);

        return res;
    }

    bool Parser::parse_bool(bool& ret)
    {
        static const std::regex
            pattern("(true)|(false)");

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = (str == "true") ? true : false;

        return res;
    }


    bool Parser::parse_string(std::string& ret)
    {
        static const std::regex
            pattern(".+");

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (res)
            ret = str;

        return res;
    }

    bool Parser::parse_bytes(std::vector<std::byte>& ret)
    {
        // Parse as a string
        std::string str;
                static const std::regex
            pattern("0x[A-F0-9]+");
        bool res = parse_pattern(pattern, str);

        if (!res) return false;
        ret.clear();

        // Make the number of characters even
        if (str.size() % 2 != 0)
            str += '0';

        for (auto i = 2LU; i < str.size(); i += 2)
            ret.push_back(std::byte(
                two_chars_to_hex(str[i], str[i+1]) ));
        
        return true;
    }

    bool Parser::parse_cell_data(Cell& ret)
    {
        int int_val;
        bool bool_val;
        std::string str_val;
        std::vector<std::byte> bytes_val;

        bool res = false;
        if ((res = parse_int(int_val)))
            ret =  Cell(int_val);

        if ((res = parse_bool(bool_val)))
            ret = Cell(bool_val);

        if ((res = parse_string(str_val)))
            ret = Cell(str_val);

        if ((res = parse_bytes(bytes_val)))
            ret = Cell(bytes_val);
        return res;
    }

    bool Parser::parse_attribute(ColumnAttribute& ret) 
    {
        static const std::regex 
            pattern{"(key)|(unique)|(autoincrement)"};

        std::string str;
        bool res = parse_pattern(pattern, str);
        if (res)
            ret = str_to_attr(str);
        return res;
    }

    bool Parser::parse_attribute_list(unsigned char& ret) 
    {
        ret = 0;

        static const std::regex 
            open_figure{"\\{"};

        static const std::regex 
            close_figure{"\\}"};

        if (!parse_pattern(open_figure)) {
            return false;
        }

        parse_whitespaces();
        
        ColumnAttribute attr;
        bool end_of_list = false;

        while (!end_of_list) {
            if (!parse_attribute(attr))
                throw AttributeException();

            ret |= attr;
            end_of_list = !parse_comma();
        }

        parse_whitespaces();
        if (!parse_pattern(close_figure))
            throw AttributeException();

        return true;
    }

    bool Parser::parse_column_type(CellType& ret)
    {
        static const std::regex 
            pattern{"(int32)|(bool)|(string(\\[[1-9][0-9]*\\])?)|(bytes(\\[[1-9][0-9]*\\])?)"};

        std::string str;
        bool res = parse_pattern(pattern, str);

        if (!res) return false;

        switch (str[1])
        {
        case 'n': ret = CellType::INT32; break;
        case 'o': ret = CellType::BOOL; break;
        case 't': ret = CellType::STRING; break;
        case 'y': ret = CellType::BYTES; break;
        }

        return true;
    }

    bool Parser::parse_column_description(Column& ret)
    {
        bool parsed_attr = false, parsed_name = false, parsed_type = false;

        unsigned char attr = 0;
        std::string name;
        CellType type;

        parsed_attr = parse_attribute_list(attr);
        parse_whitespaces();
        parsed_name = parse_name(name);

        if (parsed_attr && !parsed_name)

        if (parsed_attr && !parse_name(name))
            throw InvalidColumnDescriptionException();

        if (parsed_name && !parse_colon())
            throw InvalidColumnDescriptionException();

        parsed_type = parse_column_type(type);

        if (parsed_name && !parsed_type)
            throw InvalidColumnDescriptionException();

        ret = Column(type, name, attr);

        return parsed_attr && parsed_name && parsed_type;
    }

    bool Parser::parse_column_description_list(std::vector<Column>& ret) 
    {
        static const std::regex 
            open_par{"\\("};

        static const std::regex 
            close_par{"\\)"};

        if (!parse_pattern(open_par)) {
            return false;
        }

        parse_whitespaces();
        
        bool end_of_list = false;
        Column column;

        while (!end_of_list) {
            if (!parse_column_description(column))
                throw InvalidColumnDescriptionException();


            ret.push_back(column);
            end_of_list = !parse_comma();
        }

        parse_whitespaces();
        if (!parse_pattern(close_par))
            throw InvalidColumnDescriptionException();

        return true;
    }

    bool Parser::parse_row_ordered(std::vector<Cell>& ret)
    {
        static const std::regex 
            open_par{"\\("};

        static const std::regex 
            close_par{"\\)"};

        if (!parse_pattern(open_par)) {
            return false;
        }

        parse_whitespaces();
        
        bool end_of_list = false;

        while (!end_of_list) {
            Cell cell = Cell();
            parse_cell_data(cell);

            ret.push_back(cell);
            end_of_list = !parse_comma();
        }

        parse_whitespaces();
        if (!parse_pattern(close_par))
            throw InvalidPositionedRowException();

        return true;
    }

    bool Parser::parse_row_unordered(std::unordered_map<std::string, Cell>& ret)
    {
        static const std::regex 
            open_par{"\\("};

        static const std::regex 
            close_par{"\\)"};

        if (!parse_pattern(open_par)) {
            return false;
        }

        parse_whitespaces();
        
        bool end_of_list = false;

        while (!end_of_list) {
            Cell cell = Cell();
            std::string name = "";

            if (!parse_name(name))
                throw InvalidAssignmentRowException();

            if (!parse_equal_sign())
                throw InvalidAssignmentRowException();

            if (!parse_cell_data(cell))
                throw InvalidAssignmentRowException();

            ret[name] = cell;
            end_of_list = !parse_comma();
        }

        parse_whitespaces();
        if (!parse_pattern(close_par))
            throw InvalidAssignmentRowException();

        return true;
    }

    bool Parser::parse_column_name(std::string& ret)
    {
        static const std::regex 
            dot{"[A-Za-z0-9_\\.]+"};

        std::string str;

        bool res = parse_name(str);

        if (res)
            ret = str;

        return res;
    }


    bool Parser::parse_expression(std::unique_ptr<Expression>& ret)
    {
        static const std::regex 
            pattern(".+");

        std::string str;

        if (!parse_pattern(pattern, str))
            return false;

        std::vector<std::string> tokens = tokenize_expression(str);

        ret = std::move(parse_expression(tokens));

        return true;
    }

    // Split expression into tokens, assuming the expression is correct
    std::vector<std::string> Parser::tokenize_expression(const std::string& str)
    {
        static const std::regex 
            token_pattern(
        "([A-Za-z0-9_\\.]+)|(\\+)|(\\-)|(\\/)|(\\*)|(%)|(==)|(!=)|(>=)|(>)|(<=)|(<)|(\\&\\&)|(\\|\\|)(\\^)|(~)|(\\&)|(\\|)|(!)|(\\()|(\\))"
        );

        static const std::regex 
            space_pattern("[\\s*]");

        std::string::const_iterator 
            pos_ = str.begin(), 
            end_ = str.end();

        std::vector<std::string> res{};
        std::string token, prev;

        int parenthesis = 0;

        while (parse_pattern_static(token_pattern, pos_, end_, token)) {
            if (token.empty())
                throw IncorrectNameException();

            res.push_back(token);

            if (token == "(") parenthesis++;
            if (token == ")") 
            {
                parenthesis --;
                if (prev == "(")
                    throw EmptyExpressionException();
                if (parenthesis < 0)
                    throw IncorrectParenthesisException();
            }
            parse_pattern_static(space_pattern, pos_, end_);
            prev = std::move(token);
        }

        if (parenthesis > 0)
            throw IncorrectParenthesisException();
        return res;
    }

    std::unique_ptr<Expression> Parser::parse_expression(const std::vector<std::string>& tokens)
    {
        return parse_expression_r(tokens, tokens.begin(), tokens.end());
    }

    std::unique_ptr<Expression> Parser::parse_expression_r(const std::vector<std::string>& tokens, 
        VecPosition pos, VecPosition end)
    {
        size_t parenthesis = 0;
        size_t min_prior = (size_t)(-1);

        // skip leading parenthesis so that all unary operators will be at the begin
        VecPosition begin = pos;
        while (*begin == "(") {
            begin++;
            parenthesis += 10;
        }

        VecPosition It = begin;
        VecPosition root = end;

        while (It != end)
        {
            if (*It == "(") {
                It++;
                parenthesis += 10;
                continue;
            }

            if (*It == ")") {
                It++;
                parenthesis -= 10;
                continue;
            }

            // skip names
            if (!token_is_operation(*It)) 
            {
                It++;
                continue;
            }

            size_t prior = parenthesis + str_to_prior.at(*It);
            if (prior < min_prior) {
                min_prior = prior;
                root = It;
            }
            It++;
        }

        // Unary iterator
        if (root == begin) 
        {
            if (*root == "-")
                return std::unique_ptr<Expression>(
                    new UnaryExpression(parse_expression_r(tokens, root + 1, end), NEG));

            return std::unique_ptr<Expression>(
                    new UnaryExpression(parse_expression_r(tokens, root + 1, end), str_to_op.at(*root)));
        }

        // Value Iterator
        if (root == end) 
        {
            if (token_is_operation(*begin))
                throw IncorrectNameException();

            return std::unique_ptr<Expression>(
                    new ValueExpression(*begin));
        }


        // Binary iterator
        return std::unique_ptr<Expression>(
                    new BinaryExpression(
                        parse_expression_r(tokens, begin, root), 
                        parse_expression_r(tokens, root + 1, end), 
                        str_to_op.at(*root)));
    }

} // namespace memdb
