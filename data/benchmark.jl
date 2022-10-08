using CSV, DataFrames
using Plots

file_loc = pwd() * "\\benchmark\\n-4(new).csv";
data = DataFrame(CSV.File(file_loc, header = 1));

methods = data[!, 1];
sizes = data[!, 2];
times = data[!, 4];

u_methods = unique(methods);
u_sizes = unique(sizes);

colors = [:black, :blue, :red];
p = plot(
    title = "pthreads vs openCilk vs Multicilk (4 threads)",
    legend = :topleft,
    xlabel = "size",
    ylabel = "time[ms]"
);

for i in eachindex(u_methods)
    m = u_methods[i];
    local i_m = findall(x -> x == m, methods);
    plot!(
        p,
        sizes[i_m], times[i_m],
        linecolor = colors[i],
        label = m
    )
end
savefig(p, "./image/benchmark/4-threads.png");