#!/bin/bash
rm -rf './Tubio/mongoose.h'
rm -rf './Tubio/mongoose.c'
curl https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h > './Tubio/mongoose.h'
curl https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c > './Tubio/mongoose.c'
