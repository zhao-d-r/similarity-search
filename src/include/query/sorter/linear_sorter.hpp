//////////////////////////////////////////////////////////////////////////////
/// Copyright 2018-present Xinyan DAI<xinyan.dai@outlook.com>
///
/// permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
/// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
/// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all copies or substantial portions of
/// the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
/// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
/// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

/// @version 0.1
/// @author  Xinyan DAI
/// @contact xinyan.dai@outlook.com
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

#include "../../utils/hashers.hpp"

namespace ss {

    using namespace std;

    template<class DataType>
    class LinearSorter {
    protected:
        vector<pair<DataType, int > > _sorted_clusters;
        int index = 0;
    public:
        explicit LinearSorter(
                const int                                       clusters,
                const std::function<DataType (const int&)> &    distor) {

            _sorted_clusters.reserve(clusters);
            for(int i=0; i<clusters; ++i) {
                DataType distance = distor(i);
                _sorted_clusters.emplace_back(std::make_pair(distance, i ) );
            }

            std::sort(
                    _sorted_clusters.begin(),
                    _sorted_clusters.end(),
                    [](const pair<float, unsigned>& a, const pair<float, unsigned>& b) {
                        if (a.first != b.first)
                            return a.first < b.first;
                        else
                            return a.second < b.second;
                    });
        }

        virtual int NextBucket() {
            return _sorted_clusters[index++].second;
        }

        virtual bool NextBucketExisted() {
            return index < _sorted_clusters.size();
        }
    };
}