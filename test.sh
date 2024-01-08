#!/bin/bash

FLG=$1 && ARG=$2 && ./ft_ls $FLG $ARG > out && ls $FLG $ARG > out_ls && diff out out_ls