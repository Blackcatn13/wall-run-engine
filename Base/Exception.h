//----------------------------------------------------------------------------------
// Extern functions
// Author: Enric Vergara
//
// Description:
//This file declares a very simple exception class that contains a textual error description. This
//is used as the base class for all exception classes used in the system.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_EXCEPTION_H
#define INC_EXCEPTION_H

#include <string>

class CException
{
public:
	///Constructor that gets initialized with a text description.
	explicit			CException		(const std::string& description = "") : m_sDescription(description) {}
	
	///Constructor that gets initialized with a text description with line and file code.
	explicit			CException			(const std::string& inFile, int inLine, const std::string& inMessage);
	
	///Accessor method to retrieve the description.
	std::string		GetDescription	() const													{ return m_sDescription; }	

	///Accessor method to set the description.
	void					SetDescription	(const std::string& description) { m_sDescription = description; }

private:
	std::string	m_sDescription;	///Textual description of the error.
};

#endif //INC_EXCEPTION_H