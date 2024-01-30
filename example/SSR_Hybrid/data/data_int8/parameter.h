#ifndef PARAMETER_L{{layer}}_H
#define PARAMETER_L{{layer}}_H
#include "para_L{{layer}}.h"

static {{type}}_t matB_LUT0[L{{layer}}_size]={
{%- for i in range(num_size-1) -%} {{para_array[i]}}, {%- endfor -%} {{para_array[num_size-1]}}};

#endif
