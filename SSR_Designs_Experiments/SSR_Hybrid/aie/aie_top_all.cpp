
#include "aie_top_all.h"

aie_top_all g;

#ifdef __AIESIM__
int main(int argc, char** argv) {
    g.init();
    
    g.vit_graph0.update(g.vit_graph0.vit_0_0_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_1_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_2_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_3_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_4_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_5_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_6_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_7_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_8_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_9_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_10_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_11_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_12_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_13_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_14_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_15_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_16_0.weights_offset,0);
    g.vit_graph0.update(g.vit_graph0.vit_0_17_0.weights_offset,0);
    
    g.run(9);
    g.end();
    return 0;
}
#endif
