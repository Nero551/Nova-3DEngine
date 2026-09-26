#pragma once

template <> struct Matrix<2, 2> : BasicMatrix<4, 4>
{
};

template <> struct Matrix<3, 3> : BasicMatrix<4, 4>
{
};

template <> struct Matrix<4, 4> : BasicMatrix<4, 4>
{
};