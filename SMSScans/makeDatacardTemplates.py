import numpy as np

SRs = ["SRA","SRAb","SRB", "SRBb", "SRC", "SRCb", "SRVZBoosted","SRVZResolved","SRHZ"]

bins = {"SRA":4,"SRB":4,"SRC":3,"SRAb":4,"SRBb":4,"SRCb":3,"SRVZBoosted":5,"SRVZResolved":4,"SRHZ":3}

processes = ["sig","zjets","fsbkg","mcbkg"]


#Structure = for each process, the key contains the name of the uncertainty, and the value contains the placeholder name to go in the templates
#those keys and values containing the word bin need to get "bin" appended with the proper bin number in the templates
#In addition those with SR, need to get that part appended with the appropriate signal region

class Nuisance:
    def __init__(self,nuisance_name,placeholder_name,error_type = "lnN",additional_options = None):
        self.nuisance_name = nuisance_name
        self.placeholder_name = placeholder_name
        self.additional_options = additional_options
        self.error_type = error_type

    def get_params_for_template(self,SR = None,bin_number = -1):
        ''' Gets the name of the nuisance parameter, and the placeholder name, given SR (optional), and bin number(also optional)'''

        nuisance_name_to_return = self.nuisance_name
        placeholder_name_to_return = self.placeholder_name
        if SR:
            nuisance_name_to_return = nuisance_name_to_return.replace("_SR_","_{}_".format(SR))
        if bin_number > 0:
            nuisance_name_to_return = nuisance_name_to_return.replace("_bin","_bin{}".format(bin_number))
            placeholder_name_to_return = placeholder_name_to_return.replace("_bin","_bin{}".format(bin_number))

        if self.additional_options:
            return nuisance_name_to_return,self.error_type,placeholder_name_to_return,self.additional_options
        else:
            return nuisance_name_to_return,self.error_type,placeholder_name_to_return






nuisances = {
        "sig":[Nuisance("PUreweighting","sig_pileup_hist"),Nuisance("scale","sig_refacAndNorm_syst"),Nuisance("MET","sig_metfromFS_syst_bin"),Nuisance("lepton_FSsfs","sig_leptonFS_syst"),Nuisance("lepton_idiso","sig_leptonidiso_syst"),Nuisance("btag_HF","sig_btagheavy_syst_bin"),Nuisance("btag_LF","sig_btaglight_syst_bin"),Nuisance("lumi","sig_lumi_syst"),Nuisance("ISR","sig_isr_syst_bin"),Nuisance("sig_trig_syst_OS","sig_trig_syst"),Nuisance("sig_JES_syst_SR_bin_OS","sig_JES_syst_bin"),Nuisance("sig_stat_syst_SR_bin_OS","sig_stat_syst_bin")],

        "fsbkg":[Nuisance("fsbkg_rsfof_syst_OS","rsof_unc"),Nuisance("fsbkg_kappa_syst_OS","kappa_unc"),Nuisance("fsbkg_stat_syst_SR_bin_OS","rsfof*kappa",additional_options = "count_bin_fsbkg",error_type = "gmN")],

        "zjets":[Nuisance("zjets_norm_syst_SR_OS","zjets_norm"),Nuisance("zjets_clos_syst_SR_bin_OS","zjets_clos_bin"),Nuisance("zjets_stat_syst_SR_bin_OS","zjets_stat_bin"),Nuisance("zjets_ewk_syst_SR_bin_OS","zjets_ewk_bin")],

        "mcbkg":[Nuisance("mcbkg_scale_unc_OS","mcbkg_scale_unc_bin"),Nuisance("mcbkg_stat_SR_bin_OS","mc_stat_bin")]
        }


#Create a dictionary for every bin
#For the left most column, join the keys from all the bins to get a single key list
#For the placeholders to be filled on the right, fill a matrix, where if the key i exists for bin j, matrix[i,j+1] = placeholder, else "-"

def create_bin_dictionaries(SR):
    signal_nuisance_params = []
    zjets_nuisance_params = []
    fsbkg_nuisance_params = []
    mcbkg_nuisance_params = []

    for bin_number in range(1,bins[SR]+1):
        print("current bin_number",bin_number)
        signal_nuisance_params.append({bin_number:[i.get_params_for_template(SR,bin_number) for i in nuisances["sig"]]})
        zjets_nuisance_params.append({bin_number:[i.get_params_for_template(SR,bin_number) for i in nuisances["zjets"]]})
        fsbkg_nuisance_params.append({bin_number:[i.get_params_for_template(SR,bin_number) for i in nuisances["fsbkg"]]})
        mcbkg_nuisance_params.append({bin_number:[i.get_params_for_template(SR,bin_number) for i in nuisances["mcbkg"]]})

    return signal_nuisance_params,zjets_nuisance_params,fsbkg_nuisance_params,mcbkg_nuisance_params

def makeNuisanceParameterTable(SR):

    signal_nuisance,zjets_nuisance,fsbkg_nuisance,mcbkg_nuisance = create_bin_dictionaries(SR)

    #Create the unified list of nuisances

    nuisance_params_full_list = []

    for collection in (signal_nuisance,zjets_nuisance,fsbkg_nuisance,mcbkg_nuisance):
        for i in collection:
            for bin_number,nuisance_list in i.items():
                for nuisance in nuisance_list:
                    nuisance_params_full_list.append(nuisance[0])

    nuisance_params_full_list = np.array(list(set(nuisance_params_full_list)))

    #Create the matrix - one column per backgroud per bin, for all nuisances
    nuisance_matrix = np.zeros((len(nuisance_params_full_list),bins[SR] * 4 + 2),dtype = "<U64")
    nuisance_matrix[:] = "-"
    nuisance_matrix[:,0] = nuisance_params_full_list

    column_mapping = {"signal":0,"zjets":1,"fsbkg":2,"mcbkg":3}

    for row in range(len(nuisance_matrix[:,0])):
        nuisance_name = nuisance_matrix[row,0]
        #Loop through all bins, all backgrounds and fill stuff appropriately
        for bkg_number,collection in enumerate((signal_nuisance,zjets_nuisance,fsbkg_nuisance,mcbkg_nuisance)):
            for i in collection:
                for bin_number,nuisance_list in i.items():
                    for nuisance in nuisance_list:
                        print(str(nuisance_name),nuisance[0])
                        if str(nuisance_name) in nuisance:
                            #Fill the error type
                            if len(nuisance) == 4:
                                nuisance_matrix[row,1] = nuisance[1] + " " + "{%s}"%(nuisance[-1])
                            else:
                                nuisance_matrix[row,1] = nuisance[1]
                            #Fill the other stuff
                            print(nuisance[2])
                            nuisance_matrix[row,2+(bin_number-1) * 4 + bkg_number] = "{%s}"%(nuisance[2])

    return nuisance_matrix

def makeDatacardTemplateFile(SR):
    nuisance_parameter_table = makeNuisanceParameterTable(SR)
    f = open("Templates/{}.txt".format(SR),"w")

    #Write the headers first
    f.write("imax {} number of channels\n".format(bins[SR]))
    f.write("jmax {} number of backgrounds\n".format(3))
    f.write("kmax * number of nuisance parameters\n")
    f.write("\n")

    f.write("-----\n")
    f.write("bin\t")
    for bin_number in range(1,bins[SR]+1):
        f.write("{}_bin{}\t".format(SR,bin_number))
    f.write("\n")

    f.write("observation\t")
    for bin_number in range(1,bins[SR]+1):
        f.write("bin{}_yield\t".format(bin_number))
    f.write("\n")
    f.write("-----\n")

    f.write("bin\t")
    for bin_number in range(1,bins[SR]+1):
        for background_number in range(4):
            f.write("{}_bin{}\t".format(SR,bin_number))
    f.write("\n")

    f.write("process\t")
    for bin_number in range(1,bins[SR]+1):
        for background in ["sig","zjets","fsbkg","mcbkg"]:
            f.write("{}\t".format(background))
    f.write("\n")

    f.write("process\t")
    for bin_number in range(1,bins[SR]+1):
        for background_number in range(4):
            f.write(str(background_number)+"\t")
    f.write("\n")

    f.write("rate\t")
    for bin_number in range(1,bins[SR]+1):
        for background in ["sig","zjets","fsbkg","mcbkg"]:
            f.write("{BGbin%d_%s}\t"%(bin_number,background))
    f.write("\n")

    f.write("-----\n")

    #Write the nuisance parameter table
    for row in range(len(nuisance_parameter_table)):
        for column in range(len(nuisance_parameter_table[row])):
            f.write(nuisance_parameter_table[row,column]+"\t")
        f.write("\n")
    f.write("\n")

    f.close()



if __name__ == "__main__":
    for SR in ["SRA","SRAb","SRB","SRBb","SRC","SRCb"]:
        makeDatacardTemplateFile(SR)
