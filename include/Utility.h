#pragma once

enum class Action
{
    Invalid,
    Eval,
    Iden,
    Tran,
    Scal,
    Sub,
    Add,
    Mul,
    Comp,
    Del,
    Resize,
    Read,
    Help,
    Exit,
};

const int MAX_ALLOWED_VALUE = 1000;
const int MIN_ALLOWED_VALUE = -1024;