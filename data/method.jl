using CSV, DataFrames
using Plots

file_loc = pwd() * "\\comparison\\data2.csv";
data = DataFrame(CSV.File(file_loc, header = 1));

methods = data[!, 1];
sizes = data[!, 2];
threads = data[!, 3];
times = data[!, 4];

u_methods = unique(methods);
u_sizes = unique(sizes);
u_threads = unique(threads);

for i in eachindex(u_methods)
    m = u_methods[i];
    local i_m = findall(x -> x == m, methods);
    for j in eachindex(u_sizes)
        s = u_sizes[j];
        local indices = findall(x -> x == s, sizes[i_m]);
        local p = plot(
            threads[i_m][indices], times[i_m][indices],
            title = m * ", array size = " * string(s),
            xlabel = "Threads",
            ylabel = "Time[ms]"
        );
        savefig(p, "./image/method/" * m * "_" * string(s) * ".png");
    end
end