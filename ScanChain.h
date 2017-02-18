// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <utility>
#include <fstream>


// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TEfficiency.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

// Analysis Specific
#include "ZMET2016.cc"

// CORE
//You can not include headers!!! This is not compiled code.
#include "CoreTools/dorky.cc"
#include "CoreTools/goodrun.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2Utility.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2.cc"
// Configuration parsing
#include "ConfigParser.C"
#include "ConfigHelper.C"

using namespace std;
using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

//Global Vars
ConfigParser *conf;
int nDuplicates=0;
int num_events_veto_ttbar=0;
int num_events_veto_ttgamma=0;
bool MCTriggerEmulation = true;

vector<pair <TH1D*, TString> > g_reweight_pairs;
TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
TH1D *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36;
TEfficiency *g_pt_eff_barrel, *g_pt_eff_endcap; 
TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
TString g_sample_name;
TFile* currentFile = 0;
double g_scale_factor=1; //Holds scale factors for sample.

TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 

//set<int> inVinceNotMine = {1671877,182026,1691579,584123,919749,1637499,1956497,1038429,1276319,1475891,269306,440564,150193,591835,1864740,412753,1075606,354472,1201846,1498264,1537883,1545891,1152994,1153259,1652526,1333934,1093592,1718777,14650,508134,68097,474963,1982414,742786,556682,1886164,1611196,1559914,1230667,483404,719522,610276,1672403,944218,34871,1710872,1144948,1544257,353132,1654061,729831,172381,227100,625408,1566248,1068185,1868620,1273300,1873096,1553855,1550022,1660945,1832499,1672847,349820,1608601,630334,9104,1859361,1636849,938062,639614,1785229,1611489,1666499,1227158,1227317,11553,135776,713994,1233314,127103,756142,455254,277372,1948195,1476819,670038,1944133,1941081,916601,1796585,1830109,1487887,1890032,1564523,1978932,1187192,278190,839872,1335018,842379,355868,1174419,995752,1908016,1958574,1113094,1600282,525904,1165406,665419,668203,1957008,1398462,1389772,791755,886367,1784234,197045,769281,769397,499702,1754142,626319,671378,897686,1841626,1516470,1883436,1884038,264557,1415341,1858631,758980,196759,431803,1846737,1421249,1618690,550728,881955,528804,1917033,1974976,1568663,1175567,1753848,1789631,1182223,231669,710053,385926,704939,329723,816674,949506,89520,1896034,619515,173320,970140,1697399};

//set<int> inMineNotVince = {65603,206256,215885,352922,823174,928555,1058265,1295652,1601624,1731568,1948640};

//set<tuple<long,long,long>> inspection_set_erl = {make_tuple(446912670,281613,657),make_tuple(411131170,283306,227),make_tuple(15405861,281639,16),make_tuple(584228958,278803,323),make_tuple(14203891,284036,8),make_tuple(85470426,281639,54),make_tuple(1506544210,276831,845),make_tuple(405077299,278969,283),make_tuple(498184440,279694,276),make_tuple(474493176,283306,266),make_tuple(714008290,277127,384),make_tuple(599524472,276776,360),make_tuple(308264066,276870,231),make_tuple(268373042,281641,160),make_tuple(957484721,280385,515),make_tuple(1283027826,276870,737),make_tuple(2124587761,273725,1451),make_tuple(325760233,281641,192),make_tuple(48987779,283306,28),make_tuple(375432780,280363,237),make_tuple(2362030194,278240,1245),make_tuple(551255740,276811,314),make_tuple(368723574,281707,313),make_tuple(593025609,280385,318),make_tuple(507351115,274157,354),make_tuple(44438398,284037,23),make_tuple(226436034,283307,123),make_tuple(500405170,281707,385),make_tuple(90742531,284037,47),make_tuple(687330514,276831,405),make_tuple(769620071,274250,388),make_tuple(759167636,279715,474),make_tuple(1554238976,281707,952),make_tuple(154939362,276242,82),make_tuple(633214550,283478,447),make_tuple(949717708,283478,632),make_tuple(1599287772,277127,860),make_tuple(98194428,277180,129),make_tuple(485685969,278923,305),make_tuple(483701418,277194,337),make_tuple(370710626,278345,264),make_tuple(834697629,283307,438),make_tuple(418651650,276437,282),make_tuple(3699201302,279694,2056),make_tuple(891240730,283478,600),make_tuple(104670004,284043,67),make_tuple(138079864,283478,143),make_tuple(1241950260,277087,1150),make_tuple(1730941342,276363,965),make_tuple(1116918236,277127,589),make_tuple(223735882,283478,193),make_tuple(567304602,279760,376),make_tuple(17662543,283308,10),make_tuple(836799689,283478,568),make_tuple(50414492,275001,33),make_tuple(1538911732,276437,869),make_tuple(528794792,277094,302),make_tuple(1441761553,280249,775),make_tuple(1485274043,275890,768),make_tuple(200287523,278803,116),make_tuple(115348344,283478,124),make_tuple(2007409303,276437,1104),make_tuple(964145967,283478,640),make_tuple(185183260,275776,89),make_tuple(1202159790,276363,671),make_tuple(1429043412,283308,757),make_tuple(139193612,278239,129),make_tuple(1502419317,283308,801),make_tuple(963756973,283478,639),make_tuple(1292052857,274241,782),make_tuple(1635590919,283308,888),make_tuple(367467579,277127,193),make_tuple(337781268,279694,188),make_tuple(321796236,283478,251),make_tuple(678511248,283308,353),make_tuple(775594501,283478,530),make_tuple(656467006,276586,369),make_tuple(429645602,275309,282),make_tuple(975744985,276282,571),make_tuple(1357072292,281976,767),make_tuple(1349354042,276525,809),make_tuple(538493797,279767,310),make_tuple(278334398,283308,146),make_tuple(1369240885,281976,774),make_tuple(1248061084,276525,757),make_tuple(113704325,283548,193),make_tuple(1398973481,278875,762),make_tuple(505308078,283353,346),make_tuple(10027757,283681,7),make_tuple(405526372,277094,236),make_tuple(174669350,278193,149),make_tuple(132203912,279887,143),make_tuple(525290168,278239,343),make_tuple(578294770,277094,329),make_tuple(761910713,278239,461),make_tuple(85857592,274971,54),make_tuple(454321863,280349,239),make_tuple(889847642,280249,473),make_tuple(1259943982,280330,661),make_tuple(499046387,277096,270),make_tuple(525257256,280251,289),make_tuple(656247657,276653,425),make_tuple(111383006,283353,120),make_tuple(654001763,283682,352),make_tuple(35495309,279887,86),make_tuple(45620502,274251,29),make_tuple(191303099,275309,145),make_tuple(383715357,277069,268),make_tuple(712787009,278017,486),make_tuple(2963610076,276437,1652),make_tuple(2259751408,274388,1250),make_tuple(599583398,276831,358),make_tuple(463325727,283353,323),make_tuple(16741914,280017,9),make_tuple(979425869,278239,570),make_tuple(3609587459,276831,1987),make_tuple(461268471,278820,258),make_tuple(98711848,275774,56),make_tuple(1102076480,274999,597),make_tuple(4441699321,276525,2613),make_tuple(2758706989,276831,1538),make_tuple(391832426,275124,283),make_tuple(4661260,282034,3),make_tuple(991783679,283820,565),make_tuple(774471445,283353,507),make_tuple(242998980,276317,129),make_tuple(1840657780,280018,1070),make_tuple(2773123378,276811,1638),make_tuple(84665016,282035,37),make_tuple(367743293,279844,271),make_tuple(1145342567,283820,651),make_tuple(1710802004,283820,968),make_tuple(1351744974,276363,753),make_tuple(100009692,283353,114),make_tuple(3180471468,278808,1789),make_tuple(500139826,278017,357),make_tuple(70264911,283358,43),make_tuple(1277644982,283358,695),make_tuple(446492516,283820,261),make_tuple(2740335813,279841,1564),make_tuple(3176795912,279694,1756),make_tuple(2963652321,282037,1691),make_tuple(2188980787,283820,1241),make_tuple(1115744880,279975,676),make_tuple(1657648735,279841,975),make_tuple(915491525,283358,499),make_tuple(32884734,274241,18),make_tuple(2018924857,276525,1178),make_tuple(2085215400,276437,1145),make_tuple(73294343,277069,101),make_tuple(799372980,275836,422),make_tuple(339360581,279887,271),make_tuple(987922830,275847,761),make_tuple(2576595101,278820,1411),make_tuple(2053046069,276811,1157),make_tuple(622699685,277127,336),make_tuple(199680177,275309,150),make_tuple(1194474102,277070,637),make_tuple(1110673724,281707,718),make_tuple(960693547,276437,562),make_tuple(2383636270,282037,1366),make_tuple(800894400,278308,472),make_tuple(1759473354,279694,952),make_tuple(123985858,283359,81),make_tuple(1072792265,282037,593),make_tuple(364576876,275124,267),make_tuple(238142984,283820,146),make_tuple(1058388592,282037,585),make_tuple(725179598,276587,487),make_tuple(1746082038,278167,964),make_tuple(321459146,279071,242),make_tuple(190589499,278805,106),make_tuple(806573271,276437,480),make_tuple(1094779189,280242,598),make_tuple(525496135,278167,342),make_tuple(777114507,274241,457),make_tuple(196563738,279844,177),make_tuple(671620251,276525,439),make_tuple(223044166,278017,188),make_tuple(754672805,279931,487),make_tuple(2577054717,276363,1474),make_tuple(90816020,275284,45),make_tuple(180810773,275890,91),make_tuple(128977341,280191,69),make_tuple(484100714,280017,268),make_tuple(1510282584,276244,1040),make_tuple(1220054509,279760,722),make_tuple(80617323,281707,144),make_tuple(358180709,278017,268),make_tuple(366776019,276776,216),make_tuple(148619238,278308,139),make_tuple(2507068208,279841,1432),make_tuple(1469265912,278969,875),make_tuple(576643608,283830,335),make_tuple(826744488,278308,485),make_tuple(75273175,275067,40),make_tuple(65763233,275886,88),make_tuple(3351899453,280385,1849),make_tuple(3493689747,276811,2165),make_tuple(507367976,274999,286),make_tuple(2719080190,276437,1504),make_tuple(226887973,276542,178),make_tuple(90007284,275338,52),make_tuple(631184958,276581,402),make_tuple(1361525498,282037,754),make_tuple(1474700878,278308,856),make_tuple(202555137,276282,161),make_tuple(1478751688,279841,873),make_tuple(2408546412,283408,1388),make_tuple(934833372,279975,575),make_tuple(230938296,283865,134),make_tuple(1808223665,282092,1117),make_tuple(1726350174,280385,920),make_tuple(24881293,275832,15),make_tuple(308386114,279024,220),make_tuple(2718470159,277168,1487),make_tuple(1572199636,274999,866),make_tuple(271967709,279931,223),make_tuple(661930012,273425,532),make_tuple(464700190,279115,319),make_tuple(481519066,279931,335),make_tuple(251130289,280191,137),make_tuple(2630982821,283408,1508),make_tuple(83767665,278239,102),make_tuple(406994229,275291,241),make_tuple(528380366,275291,308),make_tuple(2252379495,279654,1185),make_tuple(69974863,276361,36),make_tuple(598678844,275068,304),make_tuple(163686738,278805,92),make_tuple(322951146,276775,249),make_tuple(1370320586,276870,785),make_tuple(923564687,278239,540),make_tuple(849015575,278167,505),make_tuple(2887013613,283408,1657),make_tuple(2003942393,278167,1111),make_tuple(2196385166,278167,1210),make_tuple(252304856,273502,240),make_tuple(48512973,275963,86),make_tuple(760122412,276950,397),make_tuple(27441029,278315,76),make_tuple(71224066,278805,40),make_tuple(1533615064,281693,805),make_tuple(661594594,279975,426),make_tuple(3207512677,277096,1860),make_tuple(120790282,275073,68),make_tuple(35471238,279844,85),make_tuple(611100260,275074,383),make_tuple(101645567,283408,67),make_tuple(2719792917,279931,1564),make_tuple(33600849,278310,24),make_tuple(855568077,283876,554),make_tuple(281857557,275832,145),make_tuple(1951383830,276242,1018),make_tuple(162276255,276659,93),make_tuple(353580600,280191,196),make_tuple(810916840,278308,477),make_tuple(548884270,283876,375),make_tuple(867571507,283408,524),make_tuple(57966889,276282,82),make_tuple(584959502,279966,393),make_tuple(668544567,277305,426),make_tuple(1457220658,279794,860),make_tuple(1881551749,279794,1086),make_tuple(52304953,282712,27),make_tuple(69282688,282712,36),make_tuple(208688687,277166,189),make_tuple(469852840,277166,317),make_tuple(984093713,276950,519),make_tuple(73708764,282730,116),make_tuple(90634618,282730,125),make_tuple(358465002,276282,247),make_tuple(761254794,276361,409),make_tuple(476721764,280330,252),make_tuple(491467528,273554,368),make_tuple(207270681,282731,109),make_tuple(105338603,282731,56),make_tuple(2844763762,276776,1727),make_tuple(25322312,276940,77),make_tuple(36201823,282732,21),make_tuple(490798625,283453,343),make_tuple(1164010648,277194,704),make_tuple(675938052,283453,460),make_tuple(623203810,275067,334),make_tuple(576996573,283453,396),make_tuple(704438652,276776,426),make_tuple(433119744,283876,302),make_tuple(893764445,283876,572),make_tuple(222366391,273447,136),make_tuple(242952528,278803,140),make_tuple(1536364729,274422,880),make_tuple(956528108,280017,554),make_tuple(1075741112,278770,559),make_tuple(436623344,282734,220),make_tuple(1313360517,278770,682),make_tuple(165237392,275344,146),make_tuple(659411107,283877,352),make_tuple(98718085,275068,48),make_tuple(101803627,276935,122),make_tuple(127587312,276948,63),make_tuple(711974880,275337,377),make_tuple(530076746,282734,266),make_tuple(1323447641,283877,702),make_tuple(1046547114,279760,634),make_tuple(111242434,279993,123),make_tuple(148431377,276317,79),make_tuple(831402395,274388,441),make_tuple(2939008113,278808,1655),make_tuple(121081925,274160,64),make_tuple(228545898,278770,121),make_tuple(55531807,274420,102),make_tuple(999957050,274969,548),make_tuple(713632411,279667,453),make_tuple(88501831,276807,99),make_tuple(1708469549,279667,980),make_tuple(265630730,274998,183),make_tuple(1895625702,283877,1047),make_tuple(998615983,278240,502),make_tuple(128981569,275832,68),make_tuple(582584222,278975,356),make_tuple(629540779,275338,362),make_tuple(1832415156,283877,1007),make_tuple(237864560,276655,122),make_tuple(321444945,278808,167),make_tuple(385938027,283884,415),make_tuple(1514852672,278808,831),make_tuple(1517902846,278808,833),make_tuple(169519666,282735,86),make_tuple(169508579,282735,86),make_tuple(937472881,276808,500),make_tuple(1228548503,277127,650),make_tuple(618843314,279588,404),make_tuple(299382109,283884,362),make_tuple(839542171,274969,458),make_tuple(508140216,277166,336),make_tuple(399293418,278315,283),make_tuple(1202988269,274422,688),make_tuple(517149012,276776,308),make_tuple(216881264,278017,185),make_tuple(289400670,278193,205),make_tuple(140868372,278315,143),make_tuple(1087396320,283885,593),make_tuple(154921517,281693,81),make_tuple(645363521,280364,340),make_tuple(981620426,275890,522),make_tuple(1597583140,283885,873),make_tuple(405055062,276808,205),make_tuple(1930186694,279975,1108),make_tuple(334200984,275370,249),make_tuple(1506904040,277168,810),make_tuple(2179114839,279931,1250),make_tuple(299386443,276315,190),make_tuple(603984247,282800,318),make_tuple(47166555,279993,90),make_tuple(704288490,279588,451),make_tuple(582040157,276097,301),make_tuple(254603687,277072,144),make_tuple(1374244490,275836,768),make_tuple(559880471,282807,284),make_tuple(431266223,282807,220),make_tuple(1207148927,283885,659),make_tuple(285732828,282807,143),make_tuple(622130933,282807,316),make_tuple(160895688,274442,87),make_tuple(2443324308,273730,2116),make_tuple(64635874,278770,35),make_tuple(829973370,276776,498),make_tuple(735092876,276834,530),make_tuple(164766327,278986,140),make_tuple(16450104,275833,9),make_tuple(95844035,276659,56),make_tuple(227074878,278770,120),make_tuple(1233403995,276811,676),make_tuple(555799453,275371,281),make_tuple(67681776,278509,109),make_tuple(638974092,278349,347),make_tuple(992485886,277148,596),make_tuple(870822952,279658,485),make_tuple(2101414645,282814,1154),make_tuple(45296145,278509,95),make_tuple(2790814579,279931,1609),make_tuple(371989162,275918,216),make_tuple(862419688,282814,443),make_tuple(60606902,276776,35),make_tuple(821321121,274316,459),make_tuple(373482955,277305,270),make_tuple(478847765,274315,222),make_tuple(155721965,275912,86),make_tuple(1935233061,276283,1057),make_tuple(99795815,278406,128),make_tuple(177679464,279029,106),make_tuple(238578205,274338,133),make_tuple(1225811473,275375,714),make_tuple(1835234049,276775,1054),make_tuple(284647538,277420,225),make_tuple(726655537,283934,441),make_tuple(951223171,283934,566),make_tuple(1854840845,274422,1072),make_tuple(1728469276,275310,1028),make_tuple(1331982311,283934,784),make_tuple(440033931,277127,233),make_tuple(644742765,280330,338),make_tuple(5316881,279685,4),make_tuple(1712716938,274422,985),make_tuple(160960764,276807,140),make_tuple(414175353,276935,305),make_tuple(698806328,276950,365),make_tuple(583416853,275310,320),make_tuple(454683658,278406,321),make_tuple(1010744879,278406,620),make_tuple(438463248,282919,225),make_tuple(694210809,275375,433),make_tuple(1570931582,280330,820),make_tuple(209494241,282922,113),make_tuple(513029908,274338,284),make_tuple(475048595,280385,255),make_tuple(423584675,277148,298),make_tuple(761226281,276437,456),make_tuple(2829917529,279694,1538),make_tuple(416324149,280251,228),make_tuple(480857476,278975,292),make_tuple(2536673734,278406,1440),make_tuple(2869977498,278406,1645),make_tuple(161376426,276810,88),make_tuple(213856231,278875,111),make_tuple(370027188,276501,224),make_tuple(258492954,273302,205),make_tuple(1804614936,283946,1098),make_tuple(292490919,283043,162),make_tuple(1566677186,275376,956),make_tuple(883968818,283043,467),make_tuple(671297808,274422,392),make_tuple(361041572,278406,269),make_tuple(1734756378,278406,1005),make_tuple(117138699,283946,147),make_tuple(582130676,283043,307),make_tuple(44378323,278017,84),make_tuple(3176251865,276870,1762),make_tuple(66390706,274968,41),make_tuple(693189517,283946,490),make_tuple(618780574,277070,341),make_tuple(1469045020,277070,795),make_tuple(123442712,277087,206),make_tuple(1064658055,279760,643),make_tuple(734054688,279794,464),make_tuple(3045408609,280385,1653),make_tuple(143422195,283052,87),make_tuple(583454337,278239,375),make_tuple(154306972,283052,94),make_tuple(642775195,279760,415),make_tuple(1384835826,280249,743),make_tuple(882486219,279794,547),make_tuple(2734404918,277096,1572),make_tuple(214852492,278017,183),make_tuple(422388965,278017,309),make_tuple(60709768,276527,44),make_tuple(3715152,280385,2),make_tuple(732779286,283946,513),make_tuple(3657447543,276501,2188),make_tuple(1026912431,279694,563),make_tuple(373716150,279760,267),make_tuple(1016441387,280330,536),make_tuple(59104726,276502,40),make_tuple(922468691,278770,479),make_tuple(479030330,283059,257),make_tuple(1328797524,278240,685),make_tuple(152316487,277071,80),make_tuple(1415811356,278808,771),make_tuple(206296403,278803,119),make_tuple(352943047,274284,172),make_tuple(327973001,280242,184),make_tuple(2353949132,283270,1387),make_tuple(146254557,276947,83),make_tuple(513456334,278820,286),make_tuple(173307472,283270,162),make_tuple(3605005450,279931,2107),make_tuple(34534627,280242,19),make_tuple(59548207,280242,32),make_tuple(541356859,279694,299),make_tuple(1413738984,283270,873),make_tuple(649780564,279667,419),make_tuple(658314162,279667,423),make_tuple(3432844987,281693,1892),make_tuple(802406967,280364,426),make_tuple(1259734041,279667,746),make_tuple(552381436,281693,291),make_tuple(702555889,281693,372),make_tuple(1232573659,277168,654),make_tuple(209036256,280330,110),make_tuple(4479888562,276831,2507),make_tuple(114095835,280249,62),make_tuple(830039201,283270,541),make_tuple(239948422,279715,197),make_tuple(38465689,276947,22),make_tuple(722603926,283270,480),make_tuple(391800362,283270,281),make_tuple(487423829,277148,335),make_tuple(81217292,280330,42),make_tuple(2037943567,279694,1101),make_tuple(311681746,281797,296),make_tuple(188928947,283283,91),make_tuple(1117345657,281797,741),make_tuple(675510075,281797,501),make_tuple(1209409113,281797,792),make_tuple(74443913,283306,40),make_tuple(11649408,283306,9)};

set<long> inspection_set = {1370320586, 761910713, 855568077};

bool printStats = false;
bool printFail = false;

/* returns two most B-like jet indicies */
pair<int, int> getMostBlike();

/*Finds the pair of B tagged jets (csv medium) with dijet mass closest to the mass of the higgs*/
pair<int,int> getClosestBPairToHiggsMass();

/*Builds MT2b from two highest CSV jets*/
double getMT2B();

/*Builds Mbb from two highest CSV jets*/
double getMbb();

/*This function gets the MT2 built out of the two Bjets in an event, no guarentee 
is made about selecting the highest csv jets*/
double getMT2ForBjets(bool select_highest_csv=false);

/*Builds MT2 for the two leading Bjets unless select_closest_higgs_mass is set, in which case it 
builds it out of the two bjets with dijet mass nearest the mass of the higgs.*/
double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false);

/*Returns boson Pt, determines whether sample is gjets or zjets first*/
double bosonPt();

/* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
double getMTLepMET(short id=0);

/* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
double getdRGammaLep(short id=0);

//=============================
// Triggers
//=============================
/*Checks that the event passes an "emulated photon trigger"*/
bool passPhotonEmulatedTrigger();

/*Ensures the event is within the efficiency plateu of the highest pt trigger it passed*/
bool passPhotonTriggers();

/*MC passes immediately, ensures data events were gathered from di-muon triggers*/
bool passMuonTriggers();

/*MC passes immediately, ensures data events were gathered from di-electron triggers*/
bool passElectronTriggers();

/*MC passes immediately, ensures data events were gathered from EMu triggers*/
bool passEMuTriggers();

/*MC passes immediately, ensures data events were gathered from SingleMu trigger*/
bool passSingleMuTriggers();

/*Helper method which chooses which above method to call. Calls EMu if the dil_flavor is emu, otherwise uses
the hyp_type to determine which to call. Events fail if they are hyp_type 2 and not tagged for emu*/
bool passLeptonHLTs();

//=============================
// Has Good Event Functions
//=============================
/*Lepton quality and Z mass cuts*/
bool hasGoodZ();

/*Photon quality cuts*/
bool hasGoodPhoton();

/*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
  It just checks muon quality stuff and then calls hasGoodPhoton()*/
bool hasGoodGammaMu();

/*Just a helper method that chooses which hasGood method to call based on the config event_type*/
bool hasGoodEvent();

//=============================
// Event Weight Assignment
//=============================

/*Goes through the chain of weight_from config options down to a config which does not have weight_from and
then adds a pair (config_name, hist_file) to the vector g_reweight_pairs.

For now this is depricated: NEEDS TO BE UPDATED WITH NEW CODE FIXES*/
void readyReweightHists();

/* Adds the vpt reweighting histogram to the g_reweight_pairs vector */
void readyVPTReweight(TString save_path);

/* Returns the trigger efficiency from g_pt_eff */
double getEff(const double &pt, const double &eta);

/*Loads the reweight hists from g_reweight_pairs and multiplies returns the weight associated with the proper
bin in the histogram*/
double getReweight();

/*This method stores fixes to the evt_scale1fb in the event of file corruptions. 
It's basically just a lookup table*/
double scale1fbFix();

/*Main function for determining the weights for each event*/
double getWeight();

/*Returns the weight associated with the photon prescales*/
double getPrescaleWeight();

//=============================
// Cuts
//=============================

/*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
bool passSignalRegionCuts();

/*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
over samples tagged as "rares". This is only neccesary for the full prediction.*/
bool passRareCuts();

/*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/
bool passSUSYSingalCuts();

/*Front end method to "Dorky" duplicate removal*/
bool isDuplicate();

/*Checks for MET filters*/
bool passMETFilters();

/*Holds baseline cuts*/
bool passBaseCut();

/*Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples*/
bool passFileSelections();

/*Obvi the event looper*/
int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);