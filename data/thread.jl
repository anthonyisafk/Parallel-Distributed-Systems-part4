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

wanted_threads = [2, 4, 8, 16];
colors = [:black, :blue];
for i in eachindex(wanted_threads)
    t = wanted_threads[i];
    local i_t = findall(x -> x == t, threads);
    local p = plot(
        title = string(t) * " threads",
        legend = :topleft,
        xlabel = "Size",
        ylabel = "Time[ms]"
    );

    for j in eachindex(u_methods)
        m = u_methods[j];
        local indices = findall(x -> x == m, methods[i_t]);
        scatter!(
            p,
            sizes[i_t][indices], times[i_t][indices],
            markercolor = colors[j],
            markersize = 6,
            label = m,
        );
        savefig(p, "./image/thread/" * string(t) * "-threads.png");
    end
end






