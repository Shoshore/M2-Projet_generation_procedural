#include "OMP.h"
#include <vector>


void OMP::compute_OMP(const Eigen::VectorXd& x, const Eigen::MatrixXd& D, int L, Eigen::VectorXd& coeff) {
    const int dim = x.rows();
    const int atom_num = D.cols();
    L = std::min({L, dim, atom_num});

    coeff = Eigen::VectorXd::Zero(atom_num);
    Eigen::VectorXd residual = x;

    std::vector<int> selected_index;
    selected_index.reserve(L);

    Eigen::MatrixXd temp(dim, L);
    Eigen::VectorXd a;

    for (int i = 0; i < L; i++) {
        Eigen::VectorXd dot_p = D.transpose() * residual;

        int max_index = 0;
        dot_p.cwiseAbs().maxCoeff(&max_index);

        selected_index.push_back(max_index);
        temp.col(i) = D.col(max_index);

        a = temp.leftCols(i + 1).colPivHouseholderQr().solve(x);
        residual = x - temp.leftCols(i + 1) * a;

        if (residual.norm() < 1e-12)
            break;
    }
    size_t size_selected_index = selected_index.size();
    for (size_t i = 0; i < size_selected_index; i++)
        coeff(selected_index[i]) = a(i);
}