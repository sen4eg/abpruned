//
// Created by arso on 20.3.23.
// does testos mb
//

#include "MatrixEvaluator.cpp"
//#include "MyLittlePerceptron.cpp"

int main(){
//    MatrixEvaluator::HelloPy();
//    MatrixEvaluator::debugBoard(1, 7);
    vector <int> arch = {64, 16, 16, 1};
    vector <int> acti = {0, 0, Sigmoid};
    MultiLayerPerceptron mlp = {arch, acti};
    MultiLayerPerceptron mlp_clone = {arch, acti};
    MultiLayerPerceptron mlp_none = {arch, acti};

    vector <double> in(64);
    for (int i = 0; i < 64; i++){
        in[i] = 1.0/(i + 1);
    }
//    const Eigen::VectorXd csgo(in);
    Eigen::VectorXd v = Eigen::VectorXd::Map(in.data(), in.size());
    Eigen::VectorXd res = mlp.forward(v);
    printf("R: %f\n", res[0]);
    double learn_rate = 0.01;
    mlp.mutate(learn_rate,0);
    res = mlp.forward(v);
    printf("R: %f\n", res[0]);

    return 0;
}