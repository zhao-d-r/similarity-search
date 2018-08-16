#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>
using std::vector;
using std::pair;
using std::unordered_set;
class BenchRecord {
public:
    
    BenchRecord(unsigned qId, const vector<pair<float, unsigned>>& nbs, bool sorted = false){
        // initialized queryId
        this->queryId = qId;

        // initialized knn
        if (sorted) {
            this->knn = nbs;
        } else {
            std::sort(this->knn.begin(), this->knn.end(),
                [](const pair<float, unsigned>& a, const pair<float, unsigned>& b) {
                    if (a.first != b.first)
                        return a.first < b.first;
                    else 
                        return a.second < b.second;
                });
            this->knn = nbs;
        }

        // initialized knnIvecs
        this->knnIvecs.reserve(this->knn.size());
        for (const auto& p : this->knn) {
            this->knnIvecs.insert(p.second);
        }
    }

    void push_back(unsigned nbid, float dist) {
        assert(dist >= this->knn.back().first);
        this->knn.push_back(std::make_pair(dist, nbid));
        this->knnIvecs.insert(nbid);
    }

    unsigned getId() const {
        return this->queryId;
    }

    const vector<pair<float, unsigned>>& getKNN() const {
        return this->knn;
    }
    
    unsigned size() {
        return this->knn.size();
    }

    float precision(const BenchRecord& other, unsigned numProbed) const {
        return precision(other.getId(), other.getKNN(), numProbed);
    }

    float recall(const BenchRecord& other) const {
        return recall(other.getId(), other.getKNN());
    }

    float error(const BenchRecord& other) const {
        return error(other.getId(), other.getKNN());
    }

private:
    unsigned queryId;
    vector<pair<float, unsigned>> knn;
    unordered_set<unsigned> knnIvecs;

    float precision(unsigned qId, const vector<pair<float, unsigned>>& givenKNN, unsigned numProbed) const {
        unsigned numMatched = numRetrieved(qId, this->extractIvecs(givenKNN));
        return (float) numMatched / numProbed;
    }

    float recall(unsigned qId, const vector<pair<float, unsigned>>& givenKNN) const {
        unsigned numMatched = numRetrieved(qId, this->extractIvecs(givenKNN));
        return numMatched / float(this->knn.size());
    }

    unsigned numRetrieved(unsigned qId, const vector<unsigned>& ivecs) const {
        assert(this->queryId == qId);
        unsigned matched = 0;
        for (vector<unsigned>::const_iterator it = ivecs.begin(); it != ivecs.end(); ++it) {
            if (this->knnIvecs.find(*it) != this->knnIvecs.end()) {
                matched++;
            }
        }
        return matched;
    }

    float error(unsigned qId, const vector<pair<float, unsigned>>& givenKNN) const {
        if (givenKNN.size() == 0) return -1;

        float error = 0;
        int size = std::min(givenKNN.size(), this->knn.size());

        int idx = 0;
        int idxGiven = 0;
        int count = 0;
        while(idx < size && idxGiven < size) {
            if (this->knn[idx].first == 0 && givenKNN[idxGiven].first > 0.000001) {
                idx++;
                continue;
            }
            if (this->knn[idx].first > 0.000001 && givenKNN[idxGiven].first < 0.000001) {
                assert(false);
            }

            if (this->knn[idx].first < 0.000001 && givenKNN[idxGiven].first < 0.000001) {
                error += 1;
            } else {
                float thisError = givenKNN[idxGiven].first / this->knn[idx].first;
                error += thisError;
            }

            count++;
            idx++;
            idxGiven++;
        }
        
        if (count == 0) return -1;
        else return error / count;
    }

    vector<unsigned> extractIvecs(const vector<pair<float, unsigned>>& givenKNN) const {
        vector<unsigned> ivecs;
        ivecs.resize(givenKNN.size());
        for (int i = 0; i < givenKNN.size(); ++i) {
            ivecs[i] = givenKNN[i].second;
        }
        return ivecs;
    }
};
