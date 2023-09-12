#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard() //dau 2 cham define scope
{
    init_grid();
}

GameBoard::~GameBoard()
{

}

void GameBoard::print()
{
    cout << "===============" << endl;
    cout << "|   | 1 2 3 4 |" << endl;
    cout << "---------------" << endl;
    for(unsigned int i = 0; i < board_.size(); ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < board_.at(i).size(); ++j)
        {
            switch(board_.at(i).at(j))
            {
            case GREEN: cout << "G "; break;
            case RED: cout << "R "; break;
            case EMPTY: cout << "o "; break;
            case UNUSED: cout << "  "; break;
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;
}

bool GameBoard::move(const Point &start, const Point &destination)
{
    Grid_type orig = board_;
    // Horizontal move only
    if(start.y == destination.y)
    {
        return horizontal_move(destination.y, start.x, destination.x);
    }

    // Button is moved step by step, and thus, we need a new start point
    // that is updated after each step.
    Point new_start = start;

    // Trying to move start point to the crossing from where to continue
    // vertically
    if(start.y % 2 == 0 and not horizontal_move(start.y, start.x, 1))
    {
        board_ = orig;
        return false;
    }
    new_start.x = 1;

    // Vertical move along the only possible column (the second one)
    if(not vertical_move(new_start.y, destination.y))
    {
        board_ = orig;
        return false;
    }
    new_start.y = destination.y;

    // Trying to move destination point to the crossing from where to continue
    // vertically
    if(destination.y % 2 == 0 and
       not horizontal_move(destination.y, new_start.x, destination.x))
    {
        board_ = orig;
        return false;
    }
    return true;
}

bool GameBoard::is_game_over()
{
    if(not is_same_color(board_.at(0), RED))
    {
        return false;
    }
    if(not is_same_color(board_.at(ROWS - 1), GREEN))
    {
        return false;
    }
    return true;
}

Slot_type GameBoard::which_slot(const Point &point)
{
    return board_.at(point.y).at(point.x);
}

bool GameBoard::is_valid_point(const Point p)
{
    if(p.x < 0 or p.x >= static_cast<int>(COLUMS))
    {
        return false;
    }
    if(p.y < 0 or p.y >= static_cast<int>(ROWS))
    {
        return false;
    }
    return true;
}

vector<vector<Slot_type> > GameBoard::get_game_board() const
{
    return board_;
}

Row_type GameBoard::init_top_or_bottom_row(Slot_type color)
{
    Row_type result;
    for(unsigned int i = 0; i < COLUMS; ++i)
    {
        result.push_back(color);
    }
    return result;
}

void GameBoard::init_grid()
{
    // Initializing top row
    board_.push_back(init_top_or_bottom_row(GREEN));

    // Setting all middle places as unused
    for(unsigned int i = 0; i < ROWS - 2; ++i)
    {
        Row_type row;
        for(unsigned int j = 0; j < COLUMS; ++j)
        {
            row.push_back(UNUSED);
        }
        board_.push_back(row);
        row.clear();
    }

    // Changing some of the places to be empty
    for(unsigned int i = 1; i < ROWS - 1; ++i)
    {
        board_.at(i).at(1) = EMPTY;
    }
    board_.at(2).at(2) = EMPTY;

    // Initializing the bottom row
    board_.push_back(init_top_or_bottom_row(RED));
}

bool GameBoard::horizontal_move(unsigned int row, unsigned int column1, unsigned int column2)
{
    // Identical columns => no move is needed
    if(column1 == column2)
    {
        return true;
    }

    // Moving from left to right (if possible),
    if(column1 < column2)
    {
        for(unsigned int i = column2; i > column1; --i)
        {
            if(board_.at(row).at(i) != EMPTY)
            {
                return false;
            }
        }
    }

    // or moving from right to left (if possible)
    else if(column2 < column1)
    {
        for(unsigned int i = column2; i < column1; ++i)
        {
            if(board_.at(row).at(i) != EMPTY)
            {
                return false;
            }
        }
    }

    // Moving the button, since it is possible
    board_.at(row).at(column2) = board_.at(row).at(column1);
    board_.at(row).at(column1) = EMPTY;
    return true;
}

bool GameBoard::vertical_move(unsigned int row1, unsigned int row2)
{
    // Identical rows => no move is needed
    if(row1 == row2)
    {
        return true;
    }

    // Moving from top to bottom (if possible),
    if(row1 < row2)
    {
        for(unsigned int i = row2; i > row1; --i)
        {
            if(board_.at(i).at(1) != EMPTY)
            {
                return false;
            }
        }
    }

    // or moving from bottom to top (if possible)
    else if(row2 < row1)
    {
        for(unsigned int i = row2; i < row1; ++i)
        {
            if(board_.at(i).at(1) != EMPTY)
            {
                return false;
            }
        }
    }

    // Moving the button, since it is possible
    board_.at(row2).at(1) = board_.at(row1).at(1);
    board_.at(row1).at(1) = EMPTY;
    return true;
}

bool GameBoard::is_same_color(const Row_type &r, Slot_type color)
{
    for(unsigned int i = 0; i < r.size(); ++i)
    {
        if(r.at(i) != color)
        {
            return false;
        }
    }
    return true;
}
