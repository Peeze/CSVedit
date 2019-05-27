#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CSV
{
    public:
        std::vector <std::vector <std::string> > table;
        std::vector <size_t> col_lens;
        std::string col_delimiter = " ";
        char sep = ',';
        char newline = '\n';
        char escape = '\\';
        char quote = '"';

        void read (const std::string&);
        void calculate_lens (void);
};

void CSV::read (const std::string& filename)
{
    std::ifstream input;
    input.open(filename);

    bool quoted = false;
    char c;
    std::ostringstream cell;
    std::vector <std::string> row;

    while (input.get(c)) {
        if (c == quote) {
            quoted = !quoted;
            cell << c;
        }
        else if (!quoted & c == sep) {
            row.push_back(cell.str());
            cell.str("");
        }
        else if (c == newline) {
            if (!quoted) {
                row.push_back(cell.str());
                table.push_back(row);
                cell.str("");
                row.clear(); // Should normally not reallocate memory which is
                             // good in this case since lines are expected to
                             // be similarly memory intensive.
            }
            else {
                cell << "\\n";
            }
        }
        else if (c == escape) {
            cell << c;
            input.get(c);
            cell << c;
        }
        else {
            cell << c;
        }
    }
    input.close();
}

void CSV::calculate_lens (void) {
    for (size_t irow = 0; irow != table.size(); irow++) {
        for (size_t icol = 0; icol != table[irow].size(); icol++) {
            if (col_lens.size() <= icol) {
                col_lens.push_back(0);
            }
            col_lens[icol] = std::max(col_lens[icol], table[irow][icol].length());
        }
    }
}

std::ostream& operator<< (std::ostream &out, CSV const& csv) {
    for (size_t irow = 0; irow != csv.table.size(); irow++) {
        for (size_t icol = 0; icol != csv.table[irow].size(); icol++) {
            out << std::string(csv.col_lens[icol] - csv.table[irow][icol].length(), ' ');
            out << csv.table[irow][icol];
            if (icol < csv.table[irow].size() - 1) {
                out << csv.delimiter;
            }
        }
        out << std::endl;
    }
}

int main ()
{
    CSV my_csv;
    my_csv.col_delimiter = " | ";
    my_csv.read("text.txt");
    my_csv.calculate_lens();
    std::cout << my_csv;
    return 0;
}
