#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class CSV
{
    public:
        std::vector <std::vector <std::string> > data;
        std::string col_delimiter = " ";
        std::vector <size_t> col_width;

        char sep = ',';
        char newline = '\n';
        char escape = '\\';
        char quote = '"';

        void calc_col_width (void);
        void read (std::istream&, const bool);
        void read (const std::string&, const bool);
        void write (std::ostream&);
        void write (const std::string&);
};

void CSV::read (std::istream& input, const bool repeated_sep = false)
{
    // repeated_sep shall say "treat consecutive delimiters as one"
    bool quoted = false;
    char c;
    std::ostringstream cell;
    std::vector <std::string> row;

    while (input.get(c)) {
        if (c == quote) {
            quoted = !quoted;
            cell << c;
        }
        else if (!quoted && c == sep) {
            if (!repeated_sep  || !cell.str().empty()) {
                row.push_back(cell.str());
                cell.str("");
            }
        }
        else if (c == newline) {
            if (!quoted) {
                row.push_back(cell.str());
                data.push_back(row);
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
    calc_col_width();
}

void CSV::read (const std::string& filename, const bool repeated_sep = false)
{
    std::ifstream input;
    input.open(filename);
    read(input, repeated_sep);
    input.close();
}

void CSV::write (std::ostream& output)
{
    for (size_t irow = 0; irow != data.size(); irow++) {
        for (size_t icol = 0; icol != data[irow].size(); icol++) {
            output << data[irow][icol];
            if (icol < data[irow].size() - 1) {
                output << sep;
            }
        }
        output << newline;
    }
}

void CSV::write (const std::string& filename)
{
    std::ofstream output;
    output.open(filename);
    write(output);
    output.close();
}

void CSV::calc_col_width (void)
{
    for (size_t irow = 0; irow != data.size(); irow++) {
        for (size_t icol = 0; icol != data[irow].size(); icol++) {
            if (col_width.size() <= icol) {
                col_width.push_back(0);
            }
            col_width[icol] = std::max(col_width[icol], data[irow][icol].length());
        }
    }
}

std::ostream& operator<< (std::ostream &out, CSV const& csv)
{
    for (size_t irow = 0; irow != csv.data.size(); irow++) {
        for (size_t icol = 0; icol != csv.data[irow].size(); icol++) {
            out << std::string(csv.col_width[icol] - csv.data[irow][icol].length(), ' ');
            out << csv.data[irow][icol];
            if (icol < csv.data[irow].size() - 1) {
                out << csv.col_delimiter;
            }
        }
        out << std::endl;
    }
}

int main (int argc, char *argv[])
{
    std::string ifilename;
    std::string ofilename;

    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " /path/to/input.csv [/path/to/output.csv]" << std::endl;
        return 1;
    }
    else if (argc <= 2) {
        ifilename = argv[1];
        ofilename = argv[1];
    }
    else {
        ifilename = argv[1];
        ofilename = argv[2];
    }

    //std::cout << ifilename << " >> " << ofilename << std::endl;

    CSV my_csv;
    my_csv.sep = ' ';
    my_csv.col_delimiter = " ] ";
    my_csv.read(ifilename, true);
    std::cout << my_csv;

    my_csv.sep = ',';
    my_csv.write(std::cout);

    return 0;
}
