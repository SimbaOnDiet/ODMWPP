# An Opinion Dynamics Model Taking Peer Pressure Into Account

LIANG Yi'en, Department of Physics, USTC

<b>This is the course work of Computation Physics A lectured by Prof. Ding</b>



ODMWPP（Opinion Dynamics Model With Peer Pressure）

Keywords: computational sociology, opinion dynamics, peer pressure

There is a report about this program in order to summit it as course work, however there is too much introduction and outcome analysis in it, hence I will just make a very brief description here without any loss of technical detail.

-------------------------------------------------
Update:

After referring to sociology works on related subject, I can see that I have completely incorrect and biased knowledge on the US 2016 Election and UK Brexit Vote. I would say peer pressure have nothing to do with US 2016 Election and UK Brexit Vote, the peer pressure assumption is completely unrealistic and such situation did not occur in US and UK. Yes, explanation 1 is the reason my assumption fails. I knew nothing about the real sociology in US and UK 2 years ago and was completely wrongly misled by the false right-wing propaganda. But still, my model might probed what such assumption might lead to (again, if it runned properly without bugs) without real-world reference. 

<b>The following content was kept just for achive and a reminder of my totally stupidity and ignorance.</b> Please keep in mind that what I have said about US 2016 Election and UK Brexit Vote are completely nonsense.

-------------------------------------------------



This model aimed to account for the big gap between pre-vote poll and voting outcome result in the US 2016 Election and UK Brexit Vote. Recent published papers investigate the affect of zealots, dynamic relationship and negative influence, however, these modifications can only explain bi-polarization but not the difference between pre-vote poll and voting outcome. 

Here, I present a model taking peer pressure into account. In this model, the mainstream media offer a prevailing opinion (vote for Hillary, vote for staying in EU), and people only share their real opinion with their intimate due to peer pressure. And they have a pretended opinion other than their real one.

A more detailed account of my model: 

1. Construct a social network, to stimulate real social relation, I give each relationship a value to value the intimacy, and two person have high intimacy with the same person tend to have high intimacy, too. Groups are also produced, which contains people non-exclusively, and more the number of same groups two people belong to, more intimacy they tend to have. 
2. Give each people an initial pretended and real opinion, here I also introduce public media agent (Have one-way and low intimacy relationship with many people) and zealots(who keep sharing their real opinion and cling to it)
3. People share their real opinion with their intimate and share their pretended opinion with the less intimate and don't share with the rest
4. If a person receive a opinion different too much from his/her own real opinion, he might be even more support his initial choice and lower the intimacy of this relationship. The intimacy of a relationship can be increase otherwise. New relationship might be created, any some relationship might be broken. More intimate the friend who shares opinion with you, the more you are affected.
5. Repeat 3 and 4 multiple times, and check the outcome.



However, the outcome did not work as I expect. The outcome of my model show perfect match of pretended and real opinion distribution. I come up with a few explanations: 

1. I did not know the real situation of US 2016 Election and Brexit Vote
2. The pre-vote poll failed to reflex the real "pretended" opinion distribution
3. "Real and pretended opinion" assumption alone isn't enough to account for the gap between forecast and outcome
4. Something went terribly wrong in my model
5. A fatal bug in my program
6. Result might be heavily related to initial configuration



I planned to fix my model and tried to save it, however I failed to spare time to finish it. After all I started it as a course work not a research project, which I have already two ongoing now. So I just put it here. It was coded in a hurry, I hope my poor programming skill and bad programming style will not bother you. If you find it interesting or have any idea about it, just feel free to contact me. If you cannot get in touch with me via GitHub, you can find my mail address at  http://home.ustc.edu.cn/~liangyien/  . If you want to cite this model, please contact me. 



References:
1. Opinion polarization by learning from social feedback. The Journal of Mathematical Sociology, pages 43 2,2019.
2. Jonah Berger and Chip Heath. Who drives divergence? identity signaling, outgroup dissimilarity, and the abandonment of cultural tastes. Journal of personality and social psychology, 95(3):593, 2008.
3. John P Curtis and Frank T Smith. The dynamics of persuasion. Int. J. Math. Models Methods Appl. Sci, 2(1):115 122, 2008.
4. Sergey Edunov, Dionysios Logothetis, Cheng Wang, Avery Ching, and Maja Kabiljo. Darwini: generating realistic large-scale social graphs. ArXiv preprint arXiv:1610.00664, 2016.
5. Rainer Hegselmann, Ulrich Krause et al. Opinion dynamics and bounded confidence models, analysis, and simulation. Journal of artificial societies and social simulation, 5(3), 2002.
6. Norman PHummon and Thomas J Fararo. The emergence of computational sociology. Journal of mathematical sociology, 20(2-3):79 87, 1995.
7. Ma sM, Flache A. Differentiation without distancing. explaining bi-polarization of opinions without negative influence. PLoS ONE, pages 8 11, 2013.
8. Hafizh A Prasetya and Tsuyoshi Murata. A model of opinion and propagation structure polarization in social media. Computational Social Networks, 7(1):1 35, 2020.
9. Rajpal H, Rosas F, Jensen H J. Tangled worldview model of opinion dynamics. Frontiers in Physics, pages 7 163, 2019.
10. Agnieszka Rychwalska and Magdalena Roszczy«ska-Kurasi«ska. Polarization on social media: when group dynamics leads to societal divides. In Proceedings of the 51st Hawaii International Conference on System Sciences. 2018.
11. Flaminio Squazzoni. Agent-based computational sociology. John Wiley & Sons, 2012.
12. SW Katarzyna, S Jozef. Opinion evolution in closed community. International Journal of Modern Physics C, pages 11 6, 2000.
13. Gunjan Verma, Ananthram Swami, and Kevin Chan. The impact of competing zealots on opinion dynamics. Physica A: Statistical Mechanics and its Applications, 395:310 331, 2014.
