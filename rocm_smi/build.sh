#!/bin/bash

gcc $1 -I/opt/rocm/rocm_smi/include -L/opt/rocm/rocm_smi/lib -lrocm_smi64
