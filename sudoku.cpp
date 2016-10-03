#include "sudoku.h"
#include "backtrack.h"

using namespace std;

// Create a new puzzle, given a vector of 81 ints
// in the range 0-9 (0 denotes an initially empty square)
Sudoku::Sudoku (std::vector<int> initialProblem)
  : initial(initialProblem), solved(false), problem(81, 9)
{}

// Attempt to solve the puzzle.
void Sudoku::solve()
{
  // Note - values 0..8 in Backtrack correspond to values 1..9 in the usual puzzle.
  solved = false;

  int unsolvable = 5 * problem.numberOfVariables();

  while ((!solved) && problem.more())
    {
       int correctCount = 0;

        for (int i = 0; i < 81; i++)
        {

            int knownIncorrect = problem.numberOfVariables();

            if (initial[i] != 0){
                // Prune off values for i less than initial
                if(problem[i] + 1 != initial[i]){
                    knownIncorrect = min(knownIncorrect,i);
                }
            }else{
    //******************************* Sudoku Rules ***************************************************

                    // Find unique number in row
                    for (int j = posByColRow(column(0),row(i)); j <= posByColRow(column(8),row(i)); j++)
                      {
                        if (problem[i] + 1 == initial[j])
                          {
                            knownIncorrect = min(knownIncorrect,i);
                          }
                        else if (problem[i] == problem[j] && i != j)
                          {
                            knownIncorrect = min(knownIncorrect, max(i,j));
                          }
                      }

                    //Find unique number in column
                    for (int j = posByColRow(column(i),row(0)); j < posByColRow(column(i),row(0)) + 73; j += 9)
                      {
                        if (problem[i] + 1 == initial[j])
                          {
                            knownIncorrect = min(knownIncorrect,i);
                          }
                        else if (problem[i] == problem[j] && i != j)
                          {
                            knownIncorrect = min(knownIncorrect, max(i,j));
                          }
                      }

                    //Find unique number in square
                    for (int j = posBySquare(square(i), 0); j < posBySquare(square(i), 0) + 21; j+=9)
                      {
                        for (int k = 0; k < 3; k++)
                          {
                            if (problem[i] + 1 == initial[j + k])
                              {
                                knownIncorrect = min(knownIncorrect,i);
                              }
                            else if (problem[i] == problem[j + k] && i != (j + k))
                              {
                                knownIncorrect = min(knownIncorrect, max(i,(j + k)));
                              }
                          }
                      }
                  }

//***************************************************************************************************

                if (knownIncorrect >= problem.numberOfVariables())
                    ++correctCount;
                else
                  problem.prune(knownIncorrect + 1);


                if(correctCount == problem.numberOfVariables())
                  {
                    solved = true;
                  }
                else
                  solved = false;

                if(problem [0]  == 0 && problem[1] == 0 )
                    --unsolvable;
        }

      if (unsolvable < 0)
        break;
    }
}



// Print the puzzle state
void Sudoku::print (std::ostream& out) const
{
  int k = 0;
  for (int line = 0; line < 9; ++line)
    {
      for (int col = 0; col < 9; ++col)
	{
	  out << problem[k]+1 << ' ';
	  if (col % 3 == 2)
	    cout << ' ';
	  k++;
	}
      cout << endl;
      if (line % 3 == 2)
	cout << endl;
    }
}



// Utility functions to interpret positions in the
// vectors in terms of the squares, rows, and columns
// of a sudoku puzzle

// Given a vector position k in the range 0..80

int Sudoku::square(int k) const
// Which of the 9 large squares:
//    0 1 2
//    3 4 5
//    6 7 8
{
  int r = row(k) / 3;
  int c = column(k) / 3;
  return c + 3 * r;
}

int Sudoku::innerSquare(int k) const
// Which of the 9 small squares with a large square:
//    0 1 2
//    3 4 5
//    6 7 8
{
  int r = row(k) % 3;
  int c = column(k) % 3;
  return c + 3 * r;
}


int Sudoku::row(int k) const
  // Which row (0..8) in the entire puzzle
{
  return k / 9;
}

int Sudoku::column(int k) const
  // Which column (0..8) in the entire puzzle
{
  return k % 9;
}

// Given a outer square # ou and an inner square # in:
int Sudoku::posBySquare(int ou, int in) const
  // returns the equivalent vector position in the range 0..80
{
  int r = (ou / 3) * 3;
  int c = (ou % 3) * 3;
  r += in / 3;
  c += in % 3;
  return posByColRow(c, r);
}

// Given a column and row
int Sudoku::posByColRow(int col, int row) const
  // returns the equivalent vector position in the range 0..80
{
  return 9 * row + col;
}
