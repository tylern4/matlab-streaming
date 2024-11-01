addpath 'build'

results = [];
port = 5558;

for c = 1:1000
    input = rand(1700);
    out = stream_data(port, input);
    results = [results; out]; %#ok<AGROW>
end

saveas(histogram(results(:,end), 100), 'histogram.png')
writematrix(results,'results.csv')
