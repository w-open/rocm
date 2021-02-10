when the frefix_dir of rocm_smi_lib installed is /opt/rocm, then you can compile then .cpp with
gcc *.c -o hello_world.cpp -I/opt/rocm/rocm_smi/include -L/opt/rocm/rocm_smi/lib -lrocm_smi64
