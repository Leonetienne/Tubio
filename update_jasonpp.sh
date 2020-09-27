#!/bin/bash
rm -rf './Tubio/external_dependencies/leonetienne/JasonPP/JasonPP.hpp'
rm -rf './Tubio/external_dependencies/leonetienne/JasonPP/JasonPP.cpp'
curl https://raw.githubusercontent.com/Leonetienne/JasonPP/master/INCLUDE/release/JasonPP.hpp > './Tubio/external_dependencies/leonetienne/JasonPP/JasonPP.hpp'
curl https://raw.githubusercontent.com/Leonetienne/JasonPP/master/INCLUDE/release/JasonPP.cpp > './Tubio/external_dependencies/leonetienne/JasonPP/JasonPP.cpp'
