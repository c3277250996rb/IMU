function accelerate = my_hfun(a0, v0, velocity, T)
    accelerate = ((velocity - v0) ./T + a0) ./ 2;

end
