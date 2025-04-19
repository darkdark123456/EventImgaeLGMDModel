#pragma once
#include <stdexcept>


struct KeyNotFindException : public std::runtime_error {

    KeyNotFindException() : std::runtime_error("Key Not Find Exception ") {


	}

    KeyNotFindException(const std::string& _Message) : std::runtime_error(_Message) {


	}

};


struct  RunTimeException : public std::runtime_error {

    RunTimeException() : std::runtime_error("Programme frame  run time exception ") {

	}

    RunTimeException(const std::string& _Message) : std::runtime_error(_Message) {


	}

};


struct FileOpenException : public std::runtime_error {

    FileOpenException() : std::runtime_error("File open exception ") {

	}

    FileOpenException(const std::string& _Message) : std::runtime_error(_Message) {


	}
};


struct OutOfRangeException : public std::runtime_error {

    OutOfRangeException() : std::runtime_error("Index  out of range exception ") {


	}

    OutOfRangeException(const std::string& _Message) : std::runtime_error(_Message) {


	}


};


struct DimConvertException : public std::runtime_error {

    DimConvertException() : std::runtime_error("Dim convert exception ") {


	}


    DimConvertException(const std::string& _Message) : std::runtime_error(_Message) {

	}
};

struct VectorNullException : public std::runtime_error {
    VectorNullException() :std::runtime_error("Vector NUll Exception") {


	}

    VectorNullException(const std::string& _Message) :std::runtime_error(_Message) {



	}

};

struct DenominatorZeroException : std::runtime_error {

    DenominatorZeroException() : std::runtime_error("Denominator Zero Exception ") {

	}

    DenominatorZeroException(const std::string& _Message) :std::runtime_error(_Message) {

	}
};



struct MatrixMultDimException : std::runtime_error {

    MatrixMultDimException() : std::runtime_error(" Matrix Mult Exception ") {


	}

    MatrixMultDimException(const std::string& _Message) : std::runtime_error(_Message) {


	}


};

struct TypeException : std::runtime_error {

    TypeException() : std::runtime_error("Type Exception") {

	}

    TypeException(const std::string& Message_) : std::runtime_error(Message_) {

	}

};


struct FftException : public std::runtime_error {

    FftException() : std::runtime_error(" FFT Exception ") {

	}

    FftException(const std::string& _Message) : std::runtime_error(_Message) {


	}

};



struct  ShapeException : std::runtime_error
{
public:
    ShapeException() : std::runtime_error("Shape exception throw ") {

	}

    ShapeException(const std::string& message) : std::runtime_error(message) {


	}

};


struct  SparseQRException  : std::runtime_error
{
public:
    SparseQRException() : std::runtime_error("Sparse QR exception throw") {

	}
    SparseQRException(const std::string& message) : std::runtime_error(message) {

	}
};


struct PlotException : std::runtime_error
{
public:
    PlotException() : std::runtime_error(u8" <--!!异常 在非第一次计算时关闭已经显示的结果图像,否则会抛出plt::show()异常!---> ") {

	}

    PlotException(const std::string& message) : std::runtime_error(message) {

	}
};
