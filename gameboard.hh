#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <vector>

using namespace std;

// Possible slots in the gameboard
enum Slot_type {RED, GREEN, EMPTY, UNUSED};

// Type of the gameboard
using Row_type = vector<Slot_type>;
using Grid_type = vector<Row_type>; // vector<vector<Slot_type>>

// Size of the gameboard
const unsigned int ROWS = 5;
const unsigned int COLUMS = 4;

// Type for the locations in the gameboard
struct Point
{
    int x; // column
    int y; // row
    bool operator==(const Point& rhs) const
    {
        return x == rhs.x and y == rhs.y;
    }
};

class GameBoard
{
public:
    GameBoard();
    ~GameBoard();

    // Prints the grid
    void print();

    // If possible, moves a button in the given grid
    // from start point to destination point.
    // A move may consists of horizontal and vertical steps.
    bool move(const Point& start, const Point& destination);

    // Checks if the game is over.
    // Returns true, if all red buttons are on the top row and all green buttons
    // are on the bottom row, otherwise returns false.
    bool is_game_over();

    // Returns the slot occupying the given point.
    Slot_type which_slot(const Point& point);

    // Checks the validity of the given point.
    // Returns true if the point is inside the game grid, otherwise returns false.
    bool is_valid_point(const Point p);

    vector<vector<Slot_type>> get_game_board() const;

private:
    // Initializes a row, all buttons with the given color.
    // Used for initializing the top and bottom rows.
    Row_type init_top_or_bottom_row(Slot_type color);

    // Initializes the game grid such that all green buttons are
    // on the top row and all red buttons are on the bottom rows.
    // Empty places and unused places are as in the figure given in the assignment.
    void init_grid();

    // If possible, makes a horizontal move in the given grid along the given row
    // from column1 to column2.
    // Returns true if the move is possible, otherwise returns false.
    bool horizontal_move(unsigned int row, unsigned int column1,
                         unsigned int column2);

    // If possible, makes a vertical move in the given grid along the second
    // column from row1 to row2.
    // Returns true if the move is possible, otherwise returns false.
    bool vertical_move(unsigned int row1, unsigned int row2);

    // Checks if all buttons on the given row are of the same (given) color.
    // Returns true, if they are of the same color, otherwise returns false.
    bool is_same_color(const Row_type& r, Slot_type color);

    // Gameboard
    vector<vector<Slot_type>> board_;
};

#endif // GAMEBOARD_HH
