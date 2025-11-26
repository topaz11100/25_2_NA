$k, j$는 $0$이상 $n$이하 정수라 하자

라그랑주 계수함수의 정의에 의해

$$
L_{n,j}(x_k) = \begin{cases}
  1 & k = j \\\\
  0 & k \neq j
\end{cases} \tag{1}
$$

---

$$
H_{n,j}(x) = (1-2(x-x_j)L^\prime_{n,j}(x_j))L^2_{n,j}(x)
$$

식 (1)에 의해 아래 두 방정식이 성립한다

$$
H_{n,j}(x_j) = (1-2(x_j-x_j)L^\prime_{n,j}(x_j))L^2_{n,j}(x_j) = L^2_{n,j}(x_j) = 1
$$

$k \neq j$ 인 $k$에 대해

$$
H_{n,j}(x_k) = (1-2(x_k-x_j)L^\prime_{n,j}(x_j))L^2_{n,j}(x_k) = 0
$$

종합하여

$$
H_{n,j}(x_k) = \begin{cases}
  1 & k = j \\\\
  0 & k \neq j
\end{cases} \tag{2}
$$

---

$$
\hat{H}_{n,j}(x) = (x - x_j)L^2_{n,j}(x)
$$

식 (1)에 의해 아래 두 방정식이 성립한다

$$
\hat{H}_{n,j}{(x_{j})} = (x_{j} - x_{j}){L^{2}}_{n,j}(x_{j}) = 0
$$

$k \neq j$ 인 $k$에 대해

$$
\hat{H}_{n,j}(x_k) = (x_k - x_j)L^{2}_{n,j}(x_k) = 0
$$

종합하여

$$
\hat{H}_{n,j}(x_k) = 0 \tag{3}
$$

---

$$
H_{2n+1}(x_k) = \sum_{j=0}^{n}{f(x_j)H_{n,j}(x_k)} + \sum_{j=0}^{n}{f^\prime(x_j)\hat{H}_{n,j}(x_k)}
$$

시그마를 풀고 식(2), (3)의 결과를 대입하면

$$
= f(x_0)H_{n,0}(x_k) + ... + f(x_k)H_{n,k}(x_k) + ... + f(x_n)H_{n,n}(x_k) = f(x_k)
$$

---

$$
H_{n,j}(x) = (1-2(x-x_j)L^\prime_{n,j}(x_j))L^2_{n,j}(x)
$$

$$
H^\prime_{n,j}(x) = (-2L^\prime_{n,j}(x_j))L^2_{n,j}(x) + (1-2(x-x_j)L^\prime_{n,j}(x_j))(2L_{n,j}(x)L^\prime_{n,j}(x))
$$

식 (1)에 의해 아래 두 방정식이 성립한다

$$
H^\prime_{n,j}(x_j) = (-2L^\prime_{n,j}(x_j))L^2_{n,j}(x_j) + (1-2(x_j-x_j)L^\prime_{n,j}(x_j))(2L_{n,j}(x_j)L^\prime_{n,j}(x_j))
$$

$$
= -2L^\prime_{n,j}(x_j) + 2L^\prime_{n,j}(x_j) = 0
$$

$k \neq j$ 인 $k$에 대해

$$
H^\prime_{n,j}(x_k) = (-2L^\prime_{n,j}(x_j))L^2_{n,j}(x_k) + (1-2(x_k-x_j)L^\prime_{n,j}(x_j))(2L_{n,j}(x_k)L^\prime_{n,j}(x_k))
$$

$$
=0 + 0 = 0
$$

종합하여

$$
H^\prime_{n,j}(x_k) = 0 \tag{4}
$$

---

$$
\hat{H}_{n,j}(x) = (x - x_j)L^2_{n,j}(x)
$$

$$
\hat{H}^\prime_{n,j}(x) = L^2_{n,j}(x) +(x - x_j)2L_{n,j}(x)L^\prime_{n,j}(x)
$$

식 (1)에 의해 아래 두 방정식이 성립한다

$$
\hat{H}^\prime_{n,j}(x_j) = L^2_{n,j}(x_j) +(x_j - x_j)2L_{n,j}(x_j)L^\prime_{n,j}(x_j) = 1
$$

$k \neq j$ 인 $k$에 대해

$$
\hat{H}^\prime_{n,j}(x_k) = L^2_{n,j}(x_k) +(x_k - x_j)2L_{n,j}(x_k)L^\prime_{n,j}(x_k) = 0
$$

종합하여

$$
\hat{H}^\prime_{n,j}(x_k) = 
\begin{cases}
  1 & k = j \\\\
  0 & k \neq j
\end{cases} \tag{5}
$$

---

$$
H_{2n+1}(x_k) = \sum_{j=0}^{n}{f(x_j)H_{n,j}(x_k)} + \sum_{j=0}^{n}{f^\prime(x_j)\hat{H}_{n,j}(x_k)}
$$

미분 법칙에 의해

$$
H^\prime_{2n+1}(x_k) = \sum_{j=0}^{n}{f(x_j)H^\prime_{n,j}(x_k)} + \sum_{j=0}^{n}{f^\prime(x_j)\hat{H}^\prime_{n,j}(x_k)}
$$

시그마를 풀고 식(4), (5)의 결과를 대입하면

$$
= f^\prime(x_0)\hat{H}^\prime_{n,0}(x_k) + ... + f^\prime(x_k)\hat{H}^\prime_{n,k}(x_k) + ... + f^\prime(x_n)\hat{H}^\prime_{n,n}(x_k) = f^\prime(x_k)
$$


