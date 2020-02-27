
import plottery.plottery as ply
import ROOT as r
import numpy as np
from root_numpy import hist2array,array2hist

fitfile = r.TFile("mlfit.root")
cov_s = (fitfile.Get("shapes_fit_s")).Get("overall_total_covar")
cov_s_array = hist2array(cov_s)
cov_s_normed_array = cov_s_array.copy()
cov_s_normed = cov_s.Clone("b_normed")

for i in range(len(cov_s_array)):
    for j in range(len(cov_s_array[i])):
        cov_s_normed_array[i,j] /= np.sqrt(cov_s_array[i,i] * cov_s_array[j,j])

array2hist(hist = cov_s_normed, array = cov_s_normed_array)

ply.plot_hist_2d(
        hist = cov_s_normed,
        options = {
            "output_name":"cov_s.pdf",
            "palette_name":"rainbow",
            "title":"Correlation for signal + background fit"
            }
        )
