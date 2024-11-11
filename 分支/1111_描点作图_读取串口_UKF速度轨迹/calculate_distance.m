function [rv, rs] = calculate_distance(a, v, s, t, k)
    v(k) = v(k - 1) + t * (a(k - 1) + a(k)) / 2;
    s(k) = s(k - 1) + t * (v(k - 1) + v(k)) / 2;
    rv = v(k);
    rs = s(k);

end
