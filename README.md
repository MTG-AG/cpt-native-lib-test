# Certification Path Validation Validation Test Tool (CPT) - Test Tool for C/C++ Library Tests


This project provides a supplementary tool for the application of the [Certification Path Validation Validation Test Tool (CPT)](https://www.bsi.bund.de/DE/Themen/Kryptografie_Kryptotechnologie/Kryptografie/CPT/cpt_node.html). The CPT is a tool for the creation of X.509 certificates and CRLs for testing purposes. It comes with a test suite that verifies the correctness of implementations of the certification path validation based on the requirements laid down in RFC 5280.

With this program, the certification path validation of C/C++ cryptographic libraries
can be tested using the X.509 certificates and CRLs generated by the CPT. The tool implements tests for Botan, OpenSSL, and mbedTLS but can
be easily extended to other libraries.

# Compilation

The tool is provided with a build system for Unix. It requires the cmake build
tool to be installed.

The compiled libraries and header files must be installed on the system and the
respective paths configured in the `CMakeLists.txt`. 

Afterwards, execute

```
$ cmake .
$ make
```

# Execution

To run the tool, call the executable as 

`$ ./build/cert_test --test_cases_dir=<path-to-certification_path_tool/output/> --target=<lib-to-test>`

where the first argument is the path to the output directory of the CPT
containing the generated test cases and <lib-to-test> may be one of "botan", "openssl" and "mbedtls".

An example call would thus be

`$ ./build/cert_test --test_cases_dir=../certification_path_tool/output/ --target=botan`

The tool then outputs the test results for all the test cases found in the CTP output
directory.
