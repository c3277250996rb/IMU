function data = read_data_from_csv(filename, my_char)
    d = readtable(filename);
    if my_char == 'x'
        d1 = d.x;
    end
    if my_char == 'y'
        d1 = d.y;
    end
    if my_char == 'z'
        d1 = d.z;
    end 

    data = d1 ./ 16.384 ./ 10;
end

