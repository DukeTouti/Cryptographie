// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import {Test} from "forge-std/Test.sol";
import {DecentralizedVoting} from "../src/DecentralizedVoting.sol";

contract DecentralizedVotingTest is Test {
	DecentralizedVoting voting;

	address account1 = makeAddr("account1");
	address account2 = makeAddr("account2");
	address account3 = makeAddr("account3");
	address account4 = makeAddr("account4");
	address account5 = makeAddr("account5");
	address account6 = makeAddr("account6");

	// Etape 1 : deploiement par le Compte 1
	function setUp() public {
		vm.prank(account1);
		voting = new DecentralizedVoting();
	}

	function _etape2() internal {
		vm.startPrank(account1);
		voting.registerVoter(account2);
		voting.registerVoter(account3);
		vm.stopPrank();
	}

	function _etape4() internal {
		_etape2();
		vm.prank(account2);
		voting.proposeCandidate(account5);
	}

	function _etape7() internal {
		_etape4();
		vm.prank(account2);
		voting.vote(account5);
	}

	function _etape10() internal {
		_etape7();
		vm.prank(account1);
		voting.endVote();
	}

	function test_Etape01_OwnerEstCompte1() public view {
		assertEq(voting.owner(), account1);
	}

	function test_Etape02_InscriptionComptes2et3() public {
		_etape2();
		(address v2,,,) = voting.voters(account2);
		(address v3,,,) = voting.voters(account3);
		assertEq(v2, account2);
		assertEq(v3, account3);
	}

	function test_Etape03_NonOwnerNePeutPasInscrire() public {
		_etape2();
		vm.prank(account2);
		vm.expectRevert("Caller is not the owner");
		voting.registerVoter(account4);
	}

	function test_Etape04_ProposerCompte5() public {
		_etape4();
		assertEq(voting.candidateList(0), account5);
	}

	function test_Etape05_DejaProposeUnCandidat() public {
		_etape4();
		vm.prank(account2);
		vm.expectRevert("Voter has already proposed a candidate");
		voting.proposeCandidate(account6);
	}

	function test_Etape06_NonInscritNePeutPasProposer() public {
		_etape4();
		vm.prank(account4);
		vm.expectRevert("Caller is not a registered voter");
		voting.proposeCandidate(account6);
	}

	function test_Etape07_VotePourCompte5() public {
		_etape7();
		(, uint count) = voting.candidates(account5);
		assertEq(count, 1);
	}

	function test_Etape08_DejaVote() public {
		_etape7();
		vm.prank(account2);
		vm.expectRevert("Voter has already voted");
		voting.vote(account5);
	}

	function test_Etape09_ResultatsAvantFin() public {
		_etape7();
		vm.prank(account3);
		vm.expectRevert("Voting has not ended yet");
		voting.getResults();
	}

	function test_Etape10_FinDuVote() public {
		_etape10();
		assertTrue(voting.votingEnded());
	}

	function test_Etape11_VoteApresFin() public {
		_etape10();
		vm.prank(account3);
		vm.expectRevert("Voting has ended");
		voting.vote(account5);
	}

	function test_Etape12_Resultats() public {
		_etape10();
		vm.prank(account4);
		(address[] memory addrs, uint[] memory counts) = voting.getResults();
		assertEq(addrs.length, 1);
		assertEq(addrs[0], account5);
		assertEq(counts[0], 1);
	}
}
