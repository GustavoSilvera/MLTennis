#ifndef NEURALNET_H
#define NEURALNET_H

#define NN_H
#include <cmath>
#include <vector>

//neural network structure and stuff

class neuron {
public:
    neuron() : val(-1) {};
    neuron(double v) : val(v) {};
    double val;//weight
};
class layer {
public:
    layer() {}
    layer(int s) : num_neurons(s) { init(false); };
    int num_neurons;
    std::vector<neuron> n;
    void init(bool empty) {
        for (int i = 0; i < num_neurons; i++) {//n.size() is fixed as neuron_size
            if (!empty) n.push_back(neuron((rand() % 100) / 100.0));//random weights for now
            else n.push_back(neuron(-1));//negative values (empty)
        }
    }
    void update_value(std::vector<double> values) {
        for (int i = 0; i < num_neurons; i++) {
            n[i].val = values[i];//random weights for now
        }
    }
    void operator=(layer L) {//no idea why i need this...
        std::vector<double> vals;
        for (int i = 0; i < num_neurons; i++) {
            vals.push_back(L.n[i].val);//random weights for now
        }
        update_value(vals);
    }
};
class net {
public:
    net(std::vector<int> network, bool sig, int w_max) {
        usingSigmoid = sig;
        weight_max = w_max;
        for (int i = 0; i < network.size(); i++) {
            num_neurons.push_back(network[i]);

        }
        num_layers = num_neurons.size();
    }
    //CNN stuff
    bool usingSigmoid;
    double weight_max;
    //rest of stuff
    int num_layers;
    std::vector<int> num_neurons;
    std::vector<layer> layers;
    std::vector<std::vector<double>> bias;//individual neurons's biases
    std::vector<std::vector<std::vector<double>>> weights;//weights from ind. neurons to all neurons in next layer
    void init() {
        num_layers = num_neurons.size();//not called???
        for (int i = 0; i < num_layers; i++) {
            layers.push_back(layer{ num_neurons[i] });
        }
        //creates random weights
        for (int i = 0; i < num_layers - 1; i++) {//for every (behind) layer [not the last one]
            std::vector<std::vector<double>> ind_neuron;
            for (int j = 0; j < layers[i].num_neurons; j++) {//for every neuron in said layer
                std::vector<double> ind_neu_weights;//individual weights
                for (int k = 0; k < layers[i + 1].num_neurons; k++) {//for every neuron in the NEXT layer
                    ind_neu_weights.push_back(0);//random from -4 to 4
                }
                ind_neuron.push_back(ind_neu_weights);
            }
            weights.push_back(ind_neuron);
        }
        //creates random biases
        for (int i = 1; i < num_layers; i++) {//for every later layer (not the first one)
            std::vector<double> ind_biases;
            for (int j = 0; j < layers[i].num_neurons; j++) {//for every neuron in said layer
                ind_biases.push_back(0);//individual 'default' biases are 0
            }
            bias.push_back(ind_biases);
        }
    }
    void operator=(net L) {//no idea why i need this...
        return;
    }
    double sigmoid(double x) {//squishification (bw 0 and 1)
        const double e = 2.71828182846;
        return(1 / (1 + pow(e, -x)));
    }
    double smooth_RelU(double x) {
        if (x < 5)	return log10f(1 + exp(x));
        return (0.4337151304 * x + 0.0058131963);
    }
    void update_layers() {
        for (int i = 1; i < num_layers; i++) {//only the next layers (last ones)
            std::vector<double> new_values;
            for (int j = 0; j < layers[i].num_neurons; j++) {//for every neuron in the next layers
                double weighted_sum = 0;
                for (int k = 0; k < layers[i - 1].num_neurons; k++) {//back to every neuron in last layer
                    weighted_sum += layers[i - 1].n[k].val * weights[i - 1][k][j];//adds to the weighted sum
                }
                if (i < layers[i].num_neurons - 1) {
                    if (usingSigmoid) new_values.push_back(sigmoid(weighted_sum + bias[i - 1][j]));
                    else new_values.push_back(smooth_RelU(weighted_sum + bias[i - 1][j]));
                }
                else {
                    new_values.push_back(weighted_sum + bias[i - 1][j]);//no sigmoid on last layer (needs to be outputs)
                }
            }
            layers[i].update_value(new_values);
        }
    }
    double compute(std::vector<double> in) {
        layers[0].update_value(in);//plugs new inputs into first (input) layer
        update_layers();
        return layers[num_layers - 1].n[0].val;
    }
    void randomize_weights() {
        for (int i = 0; i < num_layers - 1; i++) {//for every layer (except last one)
            for (int j = 0; j < layers[i].num_neurons; j++) {//for every neuron in said layer
                for (int k = 0; k < layers[i + 1].num_neurons; k++) {//for every weight its attached to
                    int rand_max = 200 * weight_max;//2 decimal places (pos/neg from weight_max)
                    weights[i][j][k] = ((rand() % rand_max) / 100.0) - weight_max;//new random from -4 to 4
                    bias[i][k] = 0;//reset biases
                }
            }
        }
    }
    void assign_weights(std::vector<std::vector<std::vector<double>>> new_w) {
        weights = new_w;//works??
    }
    void assign_biases(std::vector<std::vector<double>> new_bias) {
        bias = new_bias;//works??
    }
};
class ConvNeuralNet {
public:
    ConvNeuralNet() {
        net n_x({ 3, 4, 1 }, false, 1),
            n_y({ 3, 2, 1 }, false, 1),
            n_z({ 3, 4, 1 }, true, 1);
        network = { n_x, n_y, n_z };
        num_networks = network.size();
        //initialize all (0) weights and biases
        for (int i = 0; i < num_networks; i++) {
            network[i].init();
            const int last_indx = network[i].num_layers - 1;
            layer last = network[i].layers[last_indx];
            ideals.push_back(layer{ network[i].layers[network[i].num_layers - 1].num_neurons });
            ideals[i].init(false);
        }//initialize nets
        for (int i = 0; i < num_networks; i++) {
            std::vector<std::vector<double>> layer_biases;
            for (int j = 1; j < network[i].num_layers; j++) {
                std::vector<double> ind_biases;
                for (int k = 0; k < network[i].layers[j].num_neurons; k++) {
                    ind_biases.push_back(0);//initialize all with 0
                }
                layer_biases.push_back(ind_biases);
            }
            biases.push_back(layer_biases);
        }//empty biases
        for (int L = 0; L < num_networks; L++) {
            std::vector<std::vector<std::vector<double>>> net_weights;
            for (int i = 0; i < network[L].num_layers - 1; i++) {//for every (behind) layer [not the last one]
                std::vector<std::vector<double>> ind_neuron;
                for (int j = 0; j < network[L].layers[i].num_neurons; j++) {//for every neuron in said layer
                    std::vector<double> ind_neu_weights;//individual weights
                    for (int k = 0; k < network[L].layers[i + 1].num_neurons; k++) {//for every neuron in the NEXT layer
                        ind_neu_weights.push_back(0);//0
                    }
                    ind_neuron.push_back(ind_neu_weights);
                }
                net_weights.push_back(ind_neuron);
            }
            weights.push_back(net_weights);
        }//empty weights
         //Network: 1
         //Data: 0; Using: RelU; weight_max: 1.000000; network: {3, 4, 1};
        weights[0] = {
            {
                { 7.237154, -6.869471, -2.679543, -2.023766 },
        { 5.313370, -5.467123, -2.361199, -1.004997 },
        { 1.264198, -1.692865, -0.943796, -0.960679 }
            },
   {
       { 0.183086 },
        { -0.317261 },
        { 0.181805 },
        { -0.467466 }
   },
        };
        biases[0] = {
            { 0.014781, -0.033536, 0.002453, 0.009920 },
        { -2.872344 },
        };
        //Network: 2
        //Data: 1; Using: RelU; weight_max: 1.000000; network: {3, 2, 1};
        weights[1] = {
            {
                { 2.237256, -1.133422 },
        { 2.769581, -1.105713 },
        { 0.514166, -0.442443 }
            },
   {
       { -0.009120 },
        { 0.016793 }
   },
        };
        biases[1] = {
            { 0.000046, -0.007770 },
        { 10.655820 },
        };
        //Network: 3
        //Data: 2; Using: Sigmoid; weight_max: 1.000000; network: {3, 4, 1};
        weights[2] = {
            {
                { -10.555130, 10.314067, -9.057233, -5.766669 },
        { -8.114100, 8.815086, -7.828166, -5.069989 },
        { -2.541782, 0.879858, -2.181321, -0.239263 }
            },
   {
       { 0.436965 },
        { -0.086219 },
        { 0.221381 },
        { 0.303699 }
   },
        };
        biases[2] = {
            { -0.022248, 0.032109, -0.008500, -0.002534 },
        { 4.340648 },
        };


        for (int i = 0; i < num_networks; i++) {
            network[i].assign_weights(weights[i]);
            network[i].assign_biases(biases[i]);
        }
    }

    std::vector<net> network;
    std::vector<layer> ideals;
    std::vector<std::vector<std::vector<std::vector<double>>>> weights;//all the weights for all the networks
    std::vector<std::vector<std::vector<double>>> biases;
    //COPY PASTA

    int num_networks;
    std::vector<double> compute(std::vector<double> in) {
        std::vector<double> out;
        for (int i = 0; i < in.size(); i++) {
            out.push_back(network[i].compute(in));
        }
        return out;
    }
    void randomize_weights() {
        for (int i = 0; i < num_networks; i++) {
            network[i].randomize_weights();
        }
    }
};

#endif // NEURALNET_H
