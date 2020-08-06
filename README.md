# An Opinion Dynamics Model Taking Peer Pressure Into Account

LIANG Yi'en, Department of Physics, USTC

<b>This is the course work of Computation Physics A lectured by Prof. Ding</b>



ODMWPP（Opinion Dynamics Model With Peer Pressure）

Keywords: computational sociology, opinion dynamics, peer pressure

There is a report about this program in order to summit it as course work, however there is too much introduction and outcome analysis in it, hence I will just make a very brief description here without any loss of technical detail.



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



I planned to fix my model and tried to save it, however I failed to spare time to finish it. After all I started it as a course work not a research project, which I have already two ongoing now. So I just put it here. If you find it interesting or have any idea about it, just feel free to contact me. If you cannot get in touch with me via GitHub, you can find my mail address at  http://home.ustc.edu.cn/~liangyien/  . If you want to cite this model, please contact me. 



References:

@article{Ising,
  title={Opinion Evolution in Closed Community},
  author="{SW Katarzyna, S Jozef}",
  journal={International Journal of Modern Physics C},
  pages={11(6):1157-1165},
  year={2000}
}
@article{feedback,
  title={Opinion Polarization by Learning from Social Feedback},
  author="{S. Banisch & E. Olbrich}",
  journal={The Journal of Mathematical Sociology},
  pages={43(2): 76-103},
  year={2019}
}
@article{arguement,
  title={Tangled Worldview Model of Opinion Dynamics},
  author="{Rajpal H, Rosas F, Jensen H J.}",
  journal={Frontiers in Physics},
  pages={7: 163},
  year={2019}
}
@article{without,
  title={Differentiation without Distancing. Explaining Bi-Polarization of Opinions without Negative Influence},
  author="{Ma¨s M, Flache A}",
  journal={PLoS ONE},
  pages={8(11): e74516},
  year={2013}
}
@inproceedings{confident,
  title={Polarization on social media: when group dynamics leads to societal divides},
  author={Rychwalska, Agnieszka and Roszczy{\'n}ska-Kurasi{\'n}ska, Magdalena},
  booktitle={Proceedings of the 51st Hawaii International Conference on System Sciences},
  year={2018}
}
@article{news,
  title={A model of opinion and propagation structure polarization in social media},
  author={Prasetya, Hafizh A and Murata, Tsuyoshi},
  journal={Computational Social Networks},
  volume={7},
  number={1},
  pages={1--35},
  year={2020},
  publisher={Springer}
}
@article{zeal,
  title={The impact of competing zealots on opinion dynamics},
  author={Verma, Gunjan and Swami, Ananthram and Chan, Kevin},
  journal={Physica A: Statistical Mechanics and its Applications},
  volume={395},
  pages={310--331},
  year={2014},
  publisher={Elsevier}
}
@article{bound,
  title={Opinion dynamics and bounded confidence models, analysis, and simulation},
  author={Hegselmann, Rainer and Krause, Ulrich and others},
  journal={Journal of artificial societies and social simulation},
  volume={5},
  number={3},
  year={2002}
}
@article{complex,
  title={Dynamics of opinion formation under majority rules on complex social networks},
  author={Nguyen, Vu Xuan and Xiao, Gaoxi and Xu, Xin-Jian and Wu, Qingchu and Xia, Cheng-Yi},
  journal={Scientific Reports},
  volume={10},
  number={1},
  pages={1--9},
  year={2020},
  publisher={Nature Publishing Group}
}
@article{continuous,
  title={The dynamics of persuasion},
  author={Curtis, John P and Smith, Frank T},
  journal={Int. J. Math. Models Methods Appl. Sci},
  volume={2},
  number={1},
  pages={115--122},
  year={2008},
  publisher={Citeseer}
}
@article{private,
  title={Who drives divergence? Identity signaling, outgroup dissimilarity, and the abandonment of cultural tastes.},
  author={Berger, Jonah and Heath, Chip},
  journal={Journal of personality and social psychology},
  volume={95},
  number={3},
  pages={593},
  year={2008},
  publisher={American Psychological Association}
}
@article{network,
  title={Darwini: Generating realistic large-scale social graphs},
  author={Edunov, Sergey and Logothetis, Dionysios and Wang, Cheng and Ching, Avery and Kabiljo, Maja},
  journal={arXiv preprint arXiv:1610.00664},
  year={2016}
}
@book{agent,
  title={Agent-based computational sociology},
  author={Squazzoni, Flaminio},
  year={2012},
  publisher={John Wiley \& Sons}
}
@article{history,
  title={The emergence of computational sociology},
  author={Hummon, Norman P and Fararo, Thomas J},
  journal={Journal of mathematical sociology},
  volume={20},
  number={2-3},
  pages={79--87},
  year={1995},
  publisher={Taylor \& Francis}
}