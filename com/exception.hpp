//
// exception.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2015 Laurent Winkler lwinkler888 at gmail dot com
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef POP_EXCEPTION_HPP
#define POP_EXCEPTION_HPP

#include "com/serialize.hpp"

/// A serializable exception for poplite
namespace pop {

class exception : public std::exception
{
public:
	exception() noexcept : text("") {}
	exception(const std::string& _text) noexcept : text(_text) {}

	inline const char* what() const noexcept override {
		return text.c_str();
	}
	inline bool empty() const {
		return text.empty();
	}

	std::string text;

private:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
	{
		ar & text;
	}
};

} // namespace

#endif
