#!/bin/bash
rm -rf './Tubio/JasonPP.hpp'
rm -rf './Tubio/JasonPP.cpp'
curl https://raw.githubusercontent.com/Leonetienne/JasonPP/master/INCLUDE/release/JasonPP.hpp > './Tubio/JasonPP.hpp'
curl https://raw.githubusercontent.com/Leonetienne/JasonPP/master/INCLUDE/release/JasonPP.cpp > './Tubio/JasonPP.cpp'
