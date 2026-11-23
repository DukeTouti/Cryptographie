// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract DecentralizedVoting {
	// tout le code du TP va ici
	struct Voter {
		address voterAddress;
		bool hasVoted;
		address votedFor;
		bool hasProposed;
	}

	struct Candidate {
		address candidateAddress;
		uint voteCount;
	}
}
