#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cassert>

class Instruction
{

public:
    enum OpCode
    {
        input,
        add,
        multiply,
        divide,
        mod,
        equal,
    };

    enum Register
    {
        register_w,
        register_x,
        register_y,
        register_z,
        register_null,
    };

    Instruction(OpCode opcode, Register a, Register b, int64_t value) : opcode{opcode}, registerA{a}, 
        registerB{b}, operand(value) {};

    friend std::ostream& operator<<(std::ostream& out, const Instruction& instruction)
    {
        switch(instruction.opcode)
        {
            case (Instruction::input):
                out << "input ";
                break;
            case (Instruction::add):
                out << "add ";
                break;
            case (Instruction::multiply):
                out << "multiply ";
                break;
            case (Instruction::divide):
                out << "divide ";
                break;
            case (Instruction::mod):
                out << "mod ";
                break;
            case (Instruction::equal):
                out << "equal ";
                break;
            default:
                out << "??? ";
        }

        switch(instruction.registerA)
        {
            case (Instruction::register_w):
                out << "register_w ";
                break;
            case (Instruction::register_x):
                out << "register_x ";
                break;
            case (Instruction::register_y):
                out << "register_y ";
                break;
            case (Instruction::register_z):
                out << "register_z ";
                break;
            case (Instruction::register_null):
                out << "register_null ";
                break;
        }

        if (instruction.registerB == Instruction::register_null && instruction.opcode != Instruction::input)
        {
            out << instruction.operand;
        }
        else if ( instruction.opcode != Instruction::input )
        {
            switch(instruction.registerB)
            {
                case (Instruction::register_w):
                    out << "register_w ";
                    break;
                case (Instruction::register_x):
                    out << "register_x ";
                    break;
                case (Instruction::register_y):
                    out << "register_y ";
                    break;
                case (Instruction::register_z):
                    out << "register_z ";
                    break;
                case (Instruction::register_null):
                    out << "register_null ";
                    break;
            }
        }
        
        return out;
    }

    OpCode opcode;
    Register registerA;
    Register registerB;
    int64_t operand;
};

typedef std::vector<Instruction> Program;



Instruction::Register stringToRegister(const std::string& string)
{
    if (string == "x")
    {
        return Instruction::register_x;
    }
    else if (string == "y")
    {
        return Instruction::register_y;
    }
    else if (string == "z")
    {
        return Instruction::register_z;
    }
    else if (string == "w")
    {
        return Instruction::register_w;
    }
    else
    {
        return Instruction::register_null;
    }
}

Instruction::OpCode stringToOpcode(const std::string& string)
{
    if (string == "inp")
    {
        return Instruction::input;
    }
    else if (string == "add")
    {
        return Instruction::add;
    }
    else if (string == "mul")
    {
        return Instruction::multiply;
    }
    else if (string == "div")
    {
        return Instruction::divide;
    }
    else if (string == "mod")
    {
        return Instruction::mod;
    }
    else if (string == "eql")
    {
        return Instruction::equal;
    }
    else
    {
        std::cerr << "Invalid string to convert to opcode." << std::endl;
        assert(false);
    }

    return Instruction::input;
}

class ModelNumber
{
    public:
        ModelNumber(std::array<int, 14> number) : number{number}, ptr{0} {};
        int read()
        {
            assert(ptr < 14);
            int value = number.at(ptr);
            ptr++;
            return value;
        }

    private:
        std::array<int, 14> number;
        int64_t ptr;

};

Program readProgramFromInput(std::ifstream& is)
{
    Program program;
    std::string tmpLine;

    while (std::getline(is, tmpLine, '\n'))
    {
        std::stringstream ss(tmpLine);
        std::string stringOpcode;
        std::string stringRegA;
        std::string stringRegB;
        ss >> stringOpcode;
        ss >> stringRegA;
        ss >> stringRegB;

        Instruction::OpCode opcode = stringToOpcode(stringOpcode);
        Instruction::Register registerA, registerB;
        int64_t operand = 0;
        registerA = stringToRegister(stringRegA);
        assert(registerA != Instruction::register_null); // Register A should always be present.
        registerB = stringToRegister(stringRegB);
        if (stringRegB != "" && registerB == Instruction::register_null)
        {
            operand = std::stoi(stringRegB);
        }

        program.push_back(Instruction(opcode, registerA, registerB, operand));
    }
    
    return program;
}

// Execute the program and returns if the model number was valid or not.
bool execute(const Program& program, ModelNumber& modelNumber)
{
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;
    int64_t w = 0;

    std::unordered_map<Instruction::Register, int64_t*> regBank{
        {Instruction::register_x, &x},
        {Instruction::register_y, &y},
        {Instruction::register_z, &z},
        {Instruction::register_w, &w}
    };

    for ( const Instruction& instruction : program )
    {
        if (instruction.opcode == Instruction::input)
        {
            int64_t inputValue = modelNumber.read(); 
            *(regBank[instruction.registerA]) = inputValue;
        }
        else if (instruction.opcode == Instruction::add)
        {
            int64_t valueA = *(regBank[instruction.registerA]);
            int64_t valueB;
            if (instruction.registerB != Instruction::register_null)
                valueB = *(regBank[instruction.registerB]);
            else
                valueB = instruction.operand;

            *(regBank[instruction.registerA]) = valueA + valueB;
        }
        else if (instruction.opcode == Instruction::multiply)
        {
            int64_t valueA = *(regBank[instruction.registerA]);
            int64_t valueB;
            if (instruction.registerB != Instruction::register_null)
                valueB = *(regBank[instruction.registerB]);
            else
                valueB = instruction.operand;

            *(regBank[instruction.registerA]) = valueA * valueB;
        }
        else if (instruction.opcode == Instruction::divide)
        {
            int64_t valueA = *(regBank[instruction.registerA]);
            int64_t valueB;
            if (instruction.registerB != Instruction::register_null)
                valueB = *(regBank[instruction.registerB]);
            else
                valueB = instruction.operand;

            *(regBank[instruction.registerA]) = valueA / valueB;
        }
        else if (instruction.opcode == Instruction::mod)
        {
            int64_t valueA = *(regBank[instruction.registerA]);
            int64_t valueB;
            if (instruction.registerB != Instruction::register_null)
                valueB = *(regBank[instruction.registerB]);
            else
                valueB = instruction.operand;

            *(regBank[instruction.registerA]) = valueA % valueB;
        }
        else if (instruction.opcode == Instruction::equal)
        {
            int64_t valueA = *(regBank[instruction.registerA]);
            int64_t valueB;
            if (instruction.registerB != Instruction::register_null)
                valueB = *(regBank[instruction.registerB]);
            else
                valueB = instruction.operand;

            *(regBank[instruction.registerA]) = (int64_t)valueA == valueB;
        }
        else
        {
            std::cerr << "Error, invalid instruction, halting program." << std::endl;
            std::cin.ignore();
        }

        //std::cout << "[X] " << x << " [Y] " << y << " [Z] " << z << " [W] " << w << std::endl;
        //std::cin.ignore();
    }
    
    //std::cout << z << std::endl;
    return z == 0;
}

void decrementModelNumber(std::array<int, 14>& number, int idx)
{
    number.at(idx)--;
    if ( number.at(idx) == 0 )
    {
        number.at(idx) = 9;
        decrementModelNumber(number, idx - 1);
    }
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if (!is.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    Program program = readProgramFromInput(is);
    std::array<int, 14> number{9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

    ModelNumber modelNumber(number);
    execute(program, modelNumber);

    bool valid = false;
    uint64_t iters = 0;
    while ( !valid )
    {
        // Create new model number object from number and check if it's valid.
        ModelNumber modelNumber(number);
        valid = execute(program, modelNumber);

        if (!valid)
        {
            // Decrement the model number.
            decrementModelNumber(number, 13);
        }
        iters++;

        if ( iters % 100000 == 0 )
        {
            for (const auto& e : number)
            {
                std::cout << e;
            }
            std::cout << std::endl;
        }
    }

    for (const auto& e : number)
    {
        std::cout << e;
    }
    std::cout << std::endl;

    return 1;
}
