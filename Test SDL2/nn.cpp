// nn.cpp

#include <iostream>
#include "nn.h"
#include <math.h>

NeuralNetwork::NeuralNetwork(unsigned int input_nodes_, unsigned int hidden_nodes_, unsigned int output_nodes_) :
    nb_input_nodes(input_nodes_), nb_hidden_nodes(hidden_nodes_), nb_output_nodes(output_nodes_) {

    this->weights_ih = new Matrix(hidden_nodes_, input_nodes_);
    this->weights_ho = new Matrix(output_nodes_, hidden_nodes_);
    this->weights_ih->randomize();
    this->weights_ho->randomize();

    this->bias_h = new Matrix(hidden_nodes_, 1);
    this->bias_o = new Matrix(output_nodes_, 1);
    this->bias_h->randomize();
    this->bias_o->randomize();

    this->learning_rate = 0.075f;
}

NeuralNetwork::~NeuralNetwork() {
    delete this->weights_ih;
    delete this->weights_ho;
    delete this->bias_h;
    delete this->bias_o;
}

double NeuralNetwork::sigmoid(double x) { // Static
    return 1.0f / (1.0 + exp(-x));
}

double NeuralNetwork::dsigmoid(double x) { // Static // Sigmoid derivative
    // return NeuralNetwork::sigmoid(x) * (1.0f - NeuralNetwork::sigmoid(x));
    return x * (1.0f - x); // ???
}

Matrix NeuralNetwork::predict(Matrix m_input) {

    if (m_input.rows != this->nb_input_nodes) {
        std::cerr << "NeuralNetwork::predict() : Incorrect number of inputs !" << std::endl;
        Matrix m_null(1, 1);
        return m_null;
    }

    // Compute hidden outputs values

    Matrix m_hidden(this->nb_hidden_nodes, 1);
    m_hidden = Matrix::multiply(*(this->weights_ih), m_input);
    m_hidden.add(*(this->bias_h));
    m_hidden.map(&NeuralNetwork::sigmoid); // activation function

    // Compute output's output(s) value(s)

    Matrix m_output(this->nb_output_nodes, 1);
    m_output = Matrix::multiply(*(this->weights_ho), m_hidden);
    m_output.add(*(this->bias_o));
    m_output.map(&NeuralNetwork::sigmoid); // activation function

    return m_output;
}

void NeuralNetwork::train(Matrix m_input, Matrix m_target) {

    // Generates the Hidden Outputs

    Matrix m_hidden(this->nb_hidden_nodes, 1);
    m_hidden = Matrix::multiply(*(this->weights_ih), m_input);
    m_hidden.add(*(this->bias_h));
    m_hidden.map(&NeuralNetwork::sigmoid); // activation function

    // Generates output's output(s)

    Matrix m_output(this->nb_output_nodes, 1);
    m_output = Matrix::multiply(*(this->weights_ho), m_hidden);
    m_output.add(*(this->bias_o));
    m_output.map(&NeuralNetwork::sigmoid); // activation function

    // Calculate the error
    // ERROR = TARGETS - OUTPUTS

    Matrix m_output_errors(m_target.rows, m_target.cols);
    m_output_errors = Matrix::substract(m_target, m_output);

    // gradient = outputs * (1 - outputs)

    // Calculate output gradient
    Matrix m_output_gradients(m_output.rows, m_output.cols);
    m_output_gradients = Matrix::map(m_output, &NeuralNetwork::dsigmoid);
    m_output_gradients = Matrix::multiply(m_output_gradients, m_output_errors);
    m_output_gradients.multiply(this->learning_rate);

    // Calculate hidden -> output deltas
    Matrix m_hidden_T(m_hidden.cols, m_hidden.rows);
    m_hidden_T = m_hidden.transpose();

    Matrix m_weight_ho_deltas(m_output_gradients.rows, m_hidden_T.cols);
    m_weight_ho_deltas = Matrix::multiply(m_output_gradients, m_hidden_T);

    // Adjust the weights by deltas
    this->weights_ho->add(m_weight_ho_deltas);
    // adjust the bias by its deltas (whitch is just the gradient)
    this->bias_o->add(m_output_gradients);

    // Calculate the hidden layer errors
    Matrix w_ho_T(this->weights_ho->cols, this->weights_ho->rows);
    w_ho_T = this->weights_ho->transpose();

    Matrix m_hidden_errors(w_ho_T.rows, m_output_errors.cols);
    m_hidden_errors = Matrix::multiply(w_ho_T, m_output_errors);

    // Calculate hidden gradient
    Matrix m_hidden_gradients(m_hidden.rows, m_hidden.cols);
    m_hidden_gradients = Matrix::map(m_hidden, &NeuralNetwork::dsigmoid);

    m_hidden_gradients = Matrix::adamar_multiply(m_hidden_gradients, m_hidden_errors);

    m_hidden_gradients.multiply(this->learning_rate);

    // Calculate inputs -> hidden deltas
    Matrix m_input_T(m_input.cols, m_input.rows);
    m_input_T = m_input.transpose();

    Matrix m_weight_ih_deltas(m_hidden_gradients.rows, m_input_T.cols);
    m_weight_ih_deltas = Matrix::multiply(m_hidden_gradients, m_input_T);

    this->weights_ih->add(m_weight_ih_deltas);
    // adjust the bias by its deltas (whitch is just the gradient)
    this->bias_h->add(m_hidden_gradients);

}

void NeuralNetwork::mutate(Matrix* m) {

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (rand() % 10 < 1) {
                // printf("Mutation\n");
                double val = ((double)(rand() % 200 - 100) / 2000.0f);
                m->content[i][j] += val;
                if (m->content[i][j] > 1.0f) m->content[i][j] = 1.0f;
                if (m->content[i][j] < -1.0f) m->content[i][j] = -1.0f;
            }
        }
    }
}

void NeuralNetwork::inherit(NeuralNetwork* parent_brain) {

    *this->weights_ih = *parent_brain->weights_ih;
    // this->weights_ih->print();
    NeuralNetwork::mutate(this->weights_ih);

    *this->weights_ho = *parent_brain->weights_ho;
    NeuralNetwork::mutate(this->weights_ho);

    *this->bias_h = *parent_brain->bias_h;
    NeuralNetwork::mutate(this->bias_h);

    *this->bias_o = *parent_brain->bias_o;
    NeuralNetwork::mutate(this->bias_o);

}

