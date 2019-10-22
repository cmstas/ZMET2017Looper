#spectra_file="SMSScans/Spectra/mass_spectrum_t5zz_full.txt"
#spectra_file="SMSScans/Spectra/mass_spectrum_tchiwz.txt"
spectra_file="SMSScans/Spectra/mass_spectrum_tchiwz_CMS3_ext.txt"

n_glu_points=`cat $spectra_file | awk '{print $2}' | sort -n | uniq | wc -l`
glu_points=`cat $spectra_file | awk '{print $2}' | sort -n | uniq | xargs | tr ' ' ','`
echo $n_glu_points

echo "const int n_gluino_bins = $((n_glu_points-1));"
echo "const double gluino_bins[n_gluino_bins+1]={$glu_points};"

#Natural Binning
n_lsp_points=`cat $spectra_file | awk '{print $4}' | sort -n | uniq | wc -l`
lsp_points=`cat $spectra_file | awk '{print $4}' | sort -n | uniq | xargs | tr ' ' ','`

#Contrived Binning
#n_lsp_points=`cat $spectra_file | awk '{print $4}' | grep -v "40\." | grep -v "90\." | sort -n | uniq | tail -n+3 | wc -l`
#lsp_points=`cat $spectra_file | awk '{print $4}' | grep -v "40\." | grep -v "90\." | sort -n | uniq | tail -n+3 | xargs | tr ' ' ','`
echo $n_lsp_points

echo "const int n_lsp_bins = $((n_lsp_points-1));"
echo "const double lsp_bins[n_lsp_bins+1]={$lsp_points};"

#Natural Binning
mp=`cat $spectra_file | awk '{print "("$2","$4")"}' | xargs | tr ' ' ','`

#Contrived Binning
#mp=`cat $spectra_file | awk '{print "("$2","$4")"}' | grep -v "40\." | grep -v "90\." | grep -v ",25\." | grep -v ",50\." | xargs | tr ' ' ','`

echo "mass_spectrum=[$mp]"



