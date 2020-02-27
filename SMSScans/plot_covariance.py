import plottery.plottery as ply
import ROOT as r
import numpy as np
from root_numpy import hist2array,array2hist

fitfile = r.TFile("mlfit_b_only.root")
cov_b = (fitfile.Get("shapes_fit_b")).Get("overall_total_covar")
cov_b_array = hist2array(cov_b)
cov_b_normed_array = cov_b_array.copy()
cov_b_normed = cov_b.Clone("b_normed")

for i in range(len(cov_b_array)):
    for j in range(len(cov_b_array[i])):
        cov_b_normed_array[i,j] /= np.sqrt(cov_b_array[i,i] * cov_b_array[j,j])

array2hist(hist = cov_b_normed, array = cov_b_normed_array)

ply.plot_hist_2d(
        hist = cov_b_normed,
        options = {
            "output_name":"cov_b.pdf",
            "palette_name":"rainbow",
            "title":"Correlation for background only fit"
            }
        )
