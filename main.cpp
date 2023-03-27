//
// Created by arso on 20.3.23.
// does testos mb
//

#include "MatrixEvaluator.cpp"
//#include "MyLittlePerceptron.cpp"

boost::python::list create_list(int* arr, int size) {
    boost::python::list result;
    for (int i = 0; i < size; ++i) {
        result.append(arr[i]);
    }
    return result;
}

int main(){

    MatrixEvaluator me;
    me.debugBoard(1, 5, "board.txt");
//    int arc3h[] = {64, 16,16,1};
//    int ac3ti[] = {0, 0,1};
//    boost::python::list a = create_list(arc3h, 4);
//    boost::python::list b = create_list(ac3ti, 4);
//    me.createMLAgent(a, b);
//    me.debugBoard(1, 5, "board.txt");
//
//    vector <int> arch = {64, 16, 16, 1};
//    vector <int> acti = {0, 0, Sigmoid};
//    MultiLayerPerceptron mlp = {arch, acti};
//    MultiLayerPerceptron mlp_clone = {arch, acti};
//    MultiLayerPerceptron mlp_none = {arch, acti};
//
//    vector <double> in(64);
//    for (int i = 0; i < 64; i++){
//        in[i] = 1.0/(i + 1);
//    }
////    const Eigen::VectorXd csgo(in);
//    Eigen::VectorXd v = Eigen::VectorXd::Map(in.data(), in.size());
//    Eigen::VectorXd res = mlp.forward(v);
//    printf("R: %f\n", res[0]);
//    double learn_rate = 0.01;
//    mlp.mutate(learn_rate,0);
//    res = mlp.forward(v);
//    printf("R: %f\n", res[0]);

    return 0;
}