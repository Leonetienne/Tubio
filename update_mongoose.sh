#!/bin/bash
rm -rf './Tubio/external_dependencies/casenta/mongoose/mongoose.h'
rm -rf './Tubio/external_dependencies/casenta/mongoose/mongoose.c'
curl https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h > './Tubio/external_dependencies/casenta/mongoose/mongoose.h'
curl https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c > './Tubio/external_dependencies/casenta/mongoose/mongoose.c'
