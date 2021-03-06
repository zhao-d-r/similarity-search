////////////////////////////////////////////////////////////////////////////////
/// Copyright 2018-present Xinyan DAI<xinyan.dai@outlook.com>
///
/// permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions ofthe Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

/// @version 0.1
/// @author  Xinyan DAI
/// @contact xinyan.dai@outlook.com
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>
#include <random>
#include <unordered_map>
#include <vector>
#include "map_index.hpp"

namespace ss {

    using std::vector;

    template<class DataType>
    class E2LSHIndex : public MapIndex<DataType, vector<int > > {

    protected:
        DataType            _r;
        vector<DataType >   _b;
        
    public:
        explicit E2LSHIndex(const parameter& para) : MapIndex<DataType, vector<int > >(para), _r(para.r), _b(para.num_bit) {}

        void Train(const Matrix<DataType> &) override {

            std::default_random_engine          generator;
            std::normal_distribution<DataType > normal_dist(0.0, 1.0);

            for (int i = 0; i<this->_para.num_bit; i++) {
                for (int j=0; j<this->_para.dim; j++) {
                    this->_projectors[i][j] = normal_dist(generator);
                }
            }

            this->_r = this->_para.r;

            std::uniform_real_distribution<DataType > uniform_real_dist(0.0, this->_r);
            for (int i = 0; i<this->_para.num_bit; i++) {
                this->_b[i] = uniform_real_dist(generator);
            }
        }

    protected:

        vector<int > Quantize(const DataType *data) override {
            vector<int > value(this->_para.dim, 0);
            for (int i=0; i<this->_para.num_bit; i++) {
                DataType quantization = this->_b[i] + ss::InnerProduct(data, this->_projectors[i].data(), this->_para.dim) ;
                value[i] = std::ceil(quantization / _r);
            }
            return value;
        }
    };

} // namespace ss
