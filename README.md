# Mus Simulator
'Mus' is a spanish card game for which I wanted to estimate some statistics to help me make informed decissions while playing. This simulator deals hands and stores relevant information to compose statistics. A numerical simulation was required because analytic solutions are unfeasable and the number of possible different hand combinations is too large for an exhaustive approach (There are 40!/(16! * (4!^)4) distinct hand combinations to deal with).

This toy simulator is meant to be run once and record metrics to construct a lookup-table. A precise enough for any kind of real use can be found in "./processed_data/data_300000_samples/mus_data.ods". 