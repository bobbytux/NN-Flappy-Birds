// nn.h
#ifndef NN_H
#define NN_H


#include "matrix.h"

class NeuralNetwork {

public:

    unsigned int nb_input_nodes;    // Number of input nodes
    unsigned int nb_hidden_nodes;   // Number of hidden nodes
    unsigned int nb_output_nodes;   // Number of output nodes

    double learning_rate;

    Matrix* weights_ih;    // Weights inputs -> hidden
    Matrix* weights_ho;    // Weights hudden -> outputs
    Matrix* bias_h;        // Hidden bias
    Matrix* bias_o;        // Outputs bias

    NeuralNetwork(unsigned int input_nodes_, unsigned int hidden_nodes_, unsigned int output_nodes_);

    ~NeuralNetwork();

    static double sigmoid(double x);
    static double dsigmoid(double x);  // Sigmoid derivative

    Matrix predict(Matrix m_input);

    void train(Matrix m_input, Matrix m_target);

    static void mutate(Matrix* m);
    void inherit(NeuralNetwork* parent_brain);

};

#endif // NN_H
