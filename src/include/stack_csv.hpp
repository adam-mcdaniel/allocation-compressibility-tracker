#pragma once

#include <stack_string.hpp>
#include <stack_file.hpp>

const size_t CSV_STR_SIZE = 128;

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef StackString<CSV_STR_SIZE> CSVString;

struct CSVCell {
    CSVCell() : type(Type::EMPTY), string() {}

    enum class Type {
        STRING,
        INTEGER,
        FLOAT,
        BOOLEAN,
        POINTER,
        EMPTY,
    };
    Type type;

    union {
        StackString<CSV_STR_SIZE> string;
        int64_t integer;
        double floating_point;
        void *pointer;
        bool boolean;
    };

    CSVCell(StackString<CSV_STR_SIZE> string) : type(Type::STRING), string(string) {}
    CSVCell(int64_t integer) : type(Type::INTEGER), integer(integer) {}
    CSVCell(uint64_t integer) : type(Type::INTEGER), integer(integer) {}
    CSVCell(double floating_point) : type(Type::FLOAT), floating_point(floating_point) {}
    CSVCell(bool boolean) : type(Type::BOOLEAN), boolean(boolean) {}
    CSVCell(void *pointer) : type(Type::POINTER), pointer(pointer) {}

    StackString<CSV_STR_SIZE> to_string() const {
        switch (type) {
            case Type::STRING:
                return string;
            case Type::INTEGER:
                return StackString<CSV_STR_SIZE>::from(integer);
            case Type::FLOAT:
                return StackString<CSV_STR_SIZE>::from(floating_point);
            case Type::BOOLEAN:
                return StackString<CSV_STR_SIZE>::from(boolean);
            case Type::POINTER:
                return StackString<CSV_STR_SIZE>::from_number((uintptr_t)pointer, 16);
            case Type::EMPTY:
            default:
                return StackString<CSV_STR_SIZE>();
        }
    }

    void log() {
        // stack_logf("%s\n", to_string().c_str());
        switch (type) {
            case Type::STRING:
                stack_logf("%s", string);
                break;
            case Type::INTEGER:
                stack_logf("%d", integer);
                break;
            case Type::FLOAT:
                stack_logf("%f", floating_point);
                break;
            case Type::POINTER:
                stack_logf("%p", pointer);
                break;
            case Type::BOOLEAN:
                stack_logf("%s", boolean ? "true" : "false");
                break;
            case Type::EMPTY:
            // default:
                break;
        }
    }

    void write(StackFile &file) {
        // stack_fprintf(file, "%s\n", to_string().c_str());
        switch (type) {
            case Type::STRING:
                file.write(string);
                break;
            case Type::INTEGER:
                stack_fprintf(file, "%d", integer);
                break;
            case Type::FLOAT:
                stack_fprintf(file, "%f", floating_point);
                break;
            case Type::POINTER:
                stack_fprintf(file, "%p", pointer);
                break;
            case Type::BOOLEAN:
                stack_fprintf(file, "%s", boolean ? "true" : "false");
                break;
            case Type::EMPTY:
            // default:
                break;
        }
    }

    CSVCell& operator=(int64_t n) {
        type = Type::INTEGER;
        integer = n;
        return *this;
    }

    CSVCell& operator=(size_t n) {
        type = Type::INTEGER;
        integer = n;
        return *this;
    }

    CSVCell& operator=(void *ptr) {
        type = Type::POINTER;
        pointer = ptr;
        return *this;
    }

    CSVCell& operator=(double n) {
        type = Type::FLOAT;
        floating_point = n;
        return *this;
    }

    CSVCell& operator=(bool n) {
        type = Type::BOOLEAN;
        boolean = n;
        return *this;
    }

    CSVCell& operator=(const StackString<CSV_STR_SIZE> &string) {
        type = Type::STRING;
        this->string = string;
        return *this;
    }
    // template <size_t Size>
    // CSVCell& operator=(const StackString<Size> &string) {
    //     type = Type::STRING;
    //     this->string = string;
    //     return *this;
    // }
    
    CSVCell& operator=(const char *string) {
        type = Type::STRING;
        this->string = StackString<CSV_STR_SIZE>(string);
        return *this;
    }

    CSVCell& operator=(const CSVCell &other) {
        type = other.type;
        switch (type) {
            case Type::STRING:
                string = other.string;
                break;
            case Type::INTEGER:
                integer = other.integer;
                break;
            case Type::FLOAT:
                floating_point = other.floating_point;
                break;
            case Type::BOOLEAN:
                boolean = other.boolean;
                break;
            case Type::POINTER:
                pointer = other.pointer;
                break;
            case Type::EMPTY:
                break;
        }
        return *this;
    }

    CSVCell(const CSVCell &other) {
        *this = other;
    }
};

template <size_t Size>
class CSVRow {
private:
    StackVec<CSVCell, Size> cells;

public:
    CSVRow() : cells() {}

    template<typename T>
    void add(T t) {
        if constexpr (std::is_same<T, StackString<CSV_STR_SIZE>>::value) {
            add_string(t);
        } else if constexpr (std::is_same<T, int64_t>::value) {
            add_integer(t);
        } else if constexpr (std::is_same<T, double>::value) {
            add_float(t);
        } else if constexpr (std::is_same<T, bool>::value) {
            add_boolean(t);
        } else if constexpr (std::is_same<T, void>::value) {
            cells.push(CSVCell());
        } else {
            add_string(StackString<CSV_STR_SIZE>::from(t));
        }
    }

    template<typename T>
    void set(CSVRow<Size> &title_row, const StackString<CSV_STR_SIZE> &title, T value) {
        for (size_t i=0; i<title_row.size(); i++) {
            if (title_row[i].to_string() == title) {
                // cells[i] = CSVCell(value);
                // stack_debugf("Found title % in title row\n", title);
                (*this)[i] = value;
                return;
            }
        }
        stack_errorf("Could not find title % in title row\n", title);
    }

    void add_string(StackString<CSV_STR_SIZE> string) {
        cells.push(CSVCell(string));
    }

    void add_integer(int64_t integer) {
        cells.push(CSVCell(integer));
    }

    void add_float(double floating_point) {
        cells.push(CSVCell(floating_point));
    }
    
    void add_boolean(bool boolean) {
        cells.push(CSVCell(boolean));
    }
    
    void add(const CSVCell &cell) {
        cells.push(cell);
    }

    void log() {
        for (size_t i=0; i<size(); i++) {
            cells[i].log();
            if (i != size() - 1) {
                stack_logf(",");
            }
        }
    }

    size_t size() const {
        return cells.size();
    }

    void write(StackFile &file) {
        for (size_t i=0; i<size(); i++) {
            cells[i].write(file);
            if (i != size() - 1) {
                // file.write(StackString<16>(","));
                stack_fprintf(file, ",");
            }
        }
    }

    CSVCell& operator[](size_t col) {
        while (col >= cells.size()) {
            cells.push(CSVCell());
        }
        return cells[col];
    }

    CSVRow<Size>& operator=(const CSVRow<Size> &other) {
        cells = other.cells;
        return *this;
    }
};

template <size_t Width, size_t Length>
class CSV {
private:
    CSVRow<Width> title_row;
    StackVec<CSVRow<Width>, Length> rows;
    bool is_first_write = true;

public:
    void log() {
        for (size_t i=0; i<rows.size(); i++) {
            rows[i].log();
            stack_logf("\n");
        }
    }

    void clear() {
        rows.clear();
    }

    void write(StackFile &file) {
        stack_debugf("Writing % rows to CSV\n", rows.size());
        // stack_debugf("Writing to % with descriptor %\n", file.get_filename(), file.get_descriptor());
        // if (file.get_mode() == Mode::WRITE) {
        switch (file.get_mode()) {
        case Mode::WRITE:
            // stack_debugf("Writing to %\n", file.get_filename());
            if (is_first_write) {
                file.clear();
                title_row.write(file);
                if (title_row.size() > 0) {
                    stack_fprintf(file, "\n");
                }
                is_first_write = false;
            }
            for (size_t i=0; i<rows.size(); i++) {
                if ((i + 1) % max(rows.size() / 10, 1) == 0) {
                    stack_infof("%d percent done writing CSV\n", (int)((i + 1) * 100 / rows.size()));
                }
                rows[i].write(file);
                stack_fprintf(file, "\n");
            }
            break;
        // } else if (file.get_mode() == Mode::APPEND) {
        case Mode::APPEND:
            stack_debugf("Appending to %\n", file.get_filename());
            if (is_first_write) {
                title_row.write(file);
                if (title_row.size() > 0) {
                    stack_fprintf(file, "\n");
                }
                is_first_write = false;
            }
            for (size_t i=0; i<rows.size(); i++) {
                if (rows[i].size() == 0) {
                    continue;
                }
                // Print a percentage done
                if ((i + 1) % max(rows.size() / 10, 1) == 0) {
                    stack_infof("%d percent done writing CSV\n", (int)((i + 1) * 100 / rows.size()));
                }
                // stack_debugf("Writing row %d\n", i);
                rows[i].write(file);
                stack_fprintf(file, "\n");
            }
            break;
        default:
            stack_errorf("Not writing to % because it is not open for writing\n", file.get_filename());
            throw std::runtime_error("File not open for writing");
        }
        stack_debugf("Done writing CSV\n");
    }

    bool full() const {
        return rows.size() >= Length;
    }

    CSVRow<Width> &new_row() {
        rows.push(CSVRow<Width>());
        return last();
    }

    CSVRow<Width> &last() {
        return rows[rows.size() - 1];
    }

    CSVRow<Width> &title() {
        return title_row;
    }

    CSVRow<Width> &operator[](size_t row) {
        while (row >= rows.size()) {
            new_row();
        }
        return rows[row];
    }

    size_t size() const {
        return rows.size();
    }
};