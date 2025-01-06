function output = com2data(s)
    data = (fgetl(s));
    dataArray = strsplit(data, ',');
    % dataNumeric(i,:) = str2double(dataArray);    
    output = str2double(dataArray);  

end
