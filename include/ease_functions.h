float ease_linear(float t) {
    return t;
}

float ease_in_quad(float t) {
    return t*t;
}

float ease_out_quad(float t) {
    return t * (2 - t);
}

float ease_inout_quad(float t) {
    return (t < 0.5) ? (2*t*t) : (-1 + (4 - 2*t u) * t);
}

float ease_in_cubic(float t) {
    return t*t*t;
}

float ease_out_cubic(float t) {
    return (--t)*t*t + 1;
}

float ease_inout_cubic(float t) {
    return t<0.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
}

float ease_in_quart(float t) {
    return t*t*t*t;
}

float ease_out_quart(float t) {
    return 1-(--t)*t*t*t;
}

float ease_inout_quart(float t) {
    return t<0.5 ? 8*t*t*t*t : 1 - 8*(--t)*t*t*t;
}

float ease_in_quint(float t) {
    return t*t*t*t*t;
}

float ease_out_quint(float t) {
    return 1+(--t)*t*t*t*t;
}

float ease_inout_quint(float t) {
    return t<0.5 ? 16*t*t*t*t*t : 1+16*(--t)*t*t*t*t;
}
