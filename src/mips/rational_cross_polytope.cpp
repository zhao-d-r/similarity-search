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


#include "executor.hpp"

#include "index/cross_polytype.hpp"
#include "index/transformer/norm_range.hpp"
#include "index/transformer/simple_transformer.hpp"
#include "query/norm_range/rational_cross_polytope.hpp"

int main(int argc, char** argv) {

    parameter para;
    LoadOptions(argc, argv, para);

    using DataType    = float;
    using KeyType     = vector<int > ;
    using HashType    = ss::CrossPolytopeIndex<DataType >;
    using Transformer = ss::SimpleTransformer<DataType  >;

    using IndexType   = ss::NormRangeIndex<DataType, KeyType, HashType, Transformer >;
    using QueryType   = ss::RationalCrossPolytopeRanking<DataType, KeyType, HashType, Transformer >;
    using MetricType  = ss::IPDistance<DataType >;

    SearchIterative<DataType, IndexType, QueryType, MetricType >(para);

}

