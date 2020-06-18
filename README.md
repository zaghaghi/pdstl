# PDSTL
[![Documentation Status](https://readthedocs.org/projects/pdstl/badge/?version=latest)](https://pdstl.readthedocs.io/en/latest/?badge=latest)
[![Build Status](https://travis-ci.org/zaghaghi/pdstl.svg?branch=master)](https://travis-ci.org/zaghaghi/pdstl)

PDSTL, is a header only template library for probabilistic data structures.

# Build Example
To Build examples do as follows.
```bash
# Setup project with meson
meson build
# Build with ninja
ninja -C build -j 4
# Build docs
cd docs
make html
```

# Documentation
Read the reference documentations on [pdstl.readthedocs.io](https://pdstl.readthedocs.io/)

# Implemented Data Structures
## Membership
| Data Structure          | Insert     | Delete          |
|-------------------------|------------|-----------------|
| Bloom Filter            | Supported  | Not Supported   |
| Counting Bloom Filter   | Supported  | Supported       |
| Quotient Filter         | Supported  | Not Implemented |
| Quotient Hash Table     | Supported  | Not Implemented |
| Cuckoo Filter           | Supported  | Supported       |

## Cardinality
| Data Structure           | Insert     | Delete          |
|--------------------------|------------|-----------------|
| Linear Counting          | Supported  | Not Supported   |
| Flajolet–Martin Counting | Supported  | Not Supported   |

# References
* [Probabilistic Data Structures and Algorithms for Big Data Applications](https://pdsa.gakhov.com/) by Andrii Gakhov, 2019, ISBN: 978-3748190486 (paperback) ASIN: B07MYKTY8W (e-book)
* Fan, L., et al. (2000) “Summary cache: a scalable wide-area web cache sharing protocol”, Journal IEEE/ACM Transactions on Networking, Vol. 8 (3), pp. 281–293.
* Bender, M., et al. (2012) “Don’t Thrash: How to Cache your Hash on Flash”, Proceedings of the VLDB Endowment, Vol. 5 (11), pp. 1627–1637.
* Fan, B., et al. (2014) “Cuckoo Filter: Practically Better Than Bloom”, Proceedings of the 10th ACM International on Conference on emerging Networking Experiments and Technologies, Sydney, Australia — December 02–05, 2014, pp. 75–88, ACM New York, NY.
* Whang, K.-Y., Vander-Zanden, B.T., Taylor H.M. (1990) “A Linear-Time Probabilistic Counting Algorithm for Database Applications”, Journal ACM Transactions on Database Systems,
Vol. 15 (2), pp. 208–229.
* Flajolet, P., Martin, G.N. (1985) “Probabilistic Counting Algorithms for Data Base Applications”, Journal of Computer and System Sciences, Vol. 31 (2), pp. 182–209.
