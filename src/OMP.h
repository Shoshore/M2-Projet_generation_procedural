#ifndef OMP_H
#define OMP_H

#include <Eigen>

class OMP {
    private:

    public:
        void compute_OMP(const Eigen::VectorXd& x, const Eigen::MatrixXd& D, int L, Eigen::VectorXd& coeff);
};

#endif
