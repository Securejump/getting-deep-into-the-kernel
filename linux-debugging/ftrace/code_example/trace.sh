#!/bin/bash

sudo trace-cmd record \
        -p mmiotrace \
        -F ./main

