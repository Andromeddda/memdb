#ifndef HEADER_GUARD_CELL_H
#define HEADER_GUARD_CELL_H

#include <string>

namespace memdb 
{
	enum CellType {
		Null,
		Int32,
		Bool,
		String
	};

	// The cell of relational database with dynamic typization 
	class Cell 
	{
	public:
		// Default constructor, creates a null cell
		Cell();

		// Copy constructor
		Cell(const Cell &other);

		// Move constructor
		Cell(Cell &&other);

		// Constructors from different types
		Cell(int data);
		Cell(bool data);
		Cell(std::string data);
		Cell(const char *data);

		std::string DataToString();

		~Cell();
	protected:
		class CellData; // Abstract class for cell content
	private:
		CellData* data_; // Pointer to data, that functions polymorphically
		CellType type_;
	};

	class Cell::CellData 
	{
	public:
		CellData();
		virtual std::string ToString();
		virtual ~CellData(); // The destructor is virtual, so that "delete data_" statement is safe

		virtual CellData* Copy() = 0;
	};

} // namespace memdb

#endif // HEADER_GUARD_CELL_H
