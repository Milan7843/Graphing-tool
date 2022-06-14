#pragma once

#include <iostream>
#include <math.h>


class Node
{
public:
	// Must be pointers, otherwise size cannot be known yet
	Node* left;
	Node* right;
	double evaluate() {}

protected:

	inline Node() {}
};

class Operator : public Node
{
public:
	char type;

	inline Operator(char type) : type(type) {}
	double evaluate() {
		switch (type)
		{
			case '+':
				return left->evaluate() + right->evaluate();
			case '-':
				return left->evaluate() - right->evaluate();
			case '*':
				return left->evaluate() * right->evaluate();
			case '/':
				return left->evaluate() / right->evaluate();
			default:
				return 0.0;
		}
	}
};

class Variable : public Node
{
public:
	double value;

	inline Variable(double value) : value(value) {}
	double evaluate() {
		return value;
	}
};

class Function : public Node
{
public:
	unsigned int function;

	inline Function(unsigned int function) : function(function) {}
	double evaluate() {
		switch (function)
		{
			case 0:
				return sin(left->evaluate());
			case 1:
				return cos(left->evaluate());
			case 2:
				return tan(left->evaluate());
			case 3:
				return asin(left->evaluate());
			case 4:
				return acos(left->evaluate());
			case 5:
				return atan(left->evaluate());
			case 6:
				return log(left->evaluate());
			case 7:
				return exp(left->evaluate());
			case 8:
				return pow(left->evaluate(), right->evaluate());
			case 9:
				return sqrt(left->evaluate());
			case 10:
				return abs(left->evaluate());
			case 11:
				return floor(left->evaluate());
			case 12:
				return ceil(left->evaluate());
			case 13:
				return std::min(left->evaluate(), right->evaluate());
			case 14:
				return std::max(left->evaluate(), right->evaluate());
			default:
				return 0.0;
		}
	}
};
/*
void evaluateExpression(std::string function)
{

}
*/