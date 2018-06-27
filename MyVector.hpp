//
//  MyVector.hpp
//  NetwrixTest
//
//  Created by Александр Пахомов on 26.06.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef MyVector_hpp
#define MyVector_hpp

#include <iostream>




template<class _Tp>
class MyVector {
	
public:
	
	typedef _Tp value_type;
	
	MyVector(const std::string& name, size_t n = 0) : _vec(n), _name(name) {
		_crtCap = 1;
		checkCap();
	}
	
	MyVector(const MyVector& v) : _vec(v._vec), _crtCap(v._crtCap), _name(v._name) {}
	
	
	~MyVector() {
		
		std::cout << "!!! " << _name << " free" << std::endl;
	}
	
	void push_back(const value_type& val) {
		
		_vec.push_back(val);
		
		checkCap();
	}
	
	value_type& operator[] (size_t offset) {
		
		value_type& res = _vec[offset];
		checkCap();
		
		return res;
	}
	
	void pop_back() {
		
		_vec.pop_back();
		
		checkCap();
	}
	
	void fit() {
		
		_vec.shrink_to_fit();
		checkCap();
	}
	
	void clear() {
		
		_vec.clear();
		checkCap();
	}
	
	void resize(size_t newSize) {
		_vec.resize(newSize);
		
		checkCap();
	}
	
	
	void operator=(const MyVector& v) {
		_vec = v._vec;
		_crtCap = v._crtCap;
		_name = v._name;
	}
	
	bool empty() const  {
		return _vec.empty();
	}
	
	size_t size() const  {
		return _vec.size();
	}
	
//	const std::vector<value_type>& vec = _vec;
	
	const std::vector<value_type>& vec() const {
		return _vec;
	}
	
private:
	
	std::vector<value_type> _vec;
	size_t _crtCap = 1;
	
	std::string _name;
	
	void checkCap() {
		
		size_t newCap = _vec.capacity();
		
		if(newCap != _crtCap && (_crtCap * sizeof(value_type) >= (1024))) {
			
			std::cout << "!!! "<< _name << " cap changed, newCap == " << newCap << " resize == " << _crtCap * sizeof(value_type) / (1024) << " KB" << std::endl;
		}
		
		_crtCap = newCap;
	}
};

#endif /* MyVector_hpp */
