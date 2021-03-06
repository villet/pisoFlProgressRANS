/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "mutStandardRoughWallFunctionFvPatchScalarField.H"
#include "RASModel.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace compressible
{
namespace RASModels
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

mutStandardRoughWallFunctionFvPatchScalarField::
mutStandardRoughWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    roughnessHeight_(pTraits<scalar>::zero),
    roughnessConstant_(pTraits<scalar>::zero),
    roughnessFudgeFactor_(pTraits<scalar>::zero)
{}


mutStandardRoughWallFunctionFvPatchScalarField::
mutStandardRoughWallFunctionFvPatchScalarField
(
    const mutStandardRoughWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    roughnessHeight_(ptf.roughnessHeight_),
    roughnessConstant_(ptf.roughnessConstant_),
    roughnessFudgeFactor_(ptf.roughnessFudgeFactor_)
{}


mutStandardRoughWallFunctionFvPatchScalarField::
mutStandardRoughWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),
    roughnessHeight_(readScalar(dict.lookup("roughnessHeight"))),
    roughnessConstant_(readScalar(dict.lookup("roughnessConstant"))),
    roughnessFudgeFactor_(readScalar(dict.lookup("roughnessFudgeFactor")))
{}


mutStandardRoughWallFunctionFvPatchScalarField::
mutStandardRoughWallFunctionFvPatchScalarField
(
    const mutStandardRoughWallFunctionFvPatchScalarField& tppsf
)
:
    fixedValueFvPatchScalarField(tppsf),
    roughnessHeight_(tppsf.roughnessHeight_),
    roughnessConstant_(tppsf.roughnessConstant_),
    roughnessFudgeFactor_(tppsf.roughnessFudgeFactor_)
{}


mutStandardRoughWallFunctionFvPatchScalarField::
mutStandardRoughWallFunctionFvPatchScalarField
(
    const mutStandardRoughWallFunctionFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(tppsf, iF),
    roughnessHeight_(tppsf.roughnessHeight_),
    roughnessConstant_(tppsf.roughnessConstant_),
    roughnessFudgeFactor_(tppsf.roughnessFudgeFactor_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void mutStandardRoughWallFunctionFvPatchScalarField::evaluate
(
    const Pstream::commsTypes
)
{
    const RASModel& rasModel
        = db().lookupObject<RASModel>("RASProperties");

    const scalar kappa = rasModel.kappa().value();
    const scalar E = rasModel.E().value();
    const scalar yPlusLam = 11.225;

    // The reciprical of the distance to the adjacent cell centre.
    const scalarField& ry = patch().deltaCoeffs();

    const fvPatchVectorField& U =
        patch().lookupPatchField<volVectorField, vector>("U");

    const fvPatchScalarField& rho =
        patch().lookupPatchField<volScalarField, scalar>("rho");

    // The flow velocity at the adjacent cell centre.
    scalarField magUp = mag(U.patchInternalField() - U);

    const scalarField& muw =
        patch().lookupPatchField<volScalarField, scalar>("mu");
    scalarField& mutw = *this;

    scalarField magFaceGradU = mag(U.snGrad());

    if(roughnessHeight_ > 0.0)
    {
        // Rough Walls.
        const scalar c_1 = 1/(90 - 2.25) + roughnessConstant_;
        static const scalar c_2 = 2.25/(90 - 2.25);
        static const scalar c_3 = 2.0*atan(1.0)/log(90/2.25);
        static const scalar c_4 = c_3*log(2.25);

        //if (KsPlusBasedOnYPlus_)
        {
            // If KsPlus is based on YPlus the extra term added to the law
            // of the wall will depend on yPlus.
            forAll(mutw, facei)
            {
                const scalar magUpara = magUp[facei];
                const scalar Re = rho[facei]*magUpara/(muw[facei]*ry[facei]);
                const scalar kappaRe = kappa*Re;

                scalar yPlus = yPlusLam;
                const scalar ryPlusLam = 1.0/yPlus;

                int iter = 0;
                scalar yPlusLast = 0.0;
                scalar dKsPlusdYPlus = roughnessHeight_*ry[facei];

                // Enforce the roughnessHeight to be less than the distance to
                // the first cell centre.
                if(dKsPlusdYPlus > 1)
                {
                    dKsPlusdYPlus = 1;
                }

                // Fudge factor to get results to be similar to fluent
                // (at least difference between rough and smooth).
                dKsPlusdYPlus *= roughnessFudgeFactor_;

                do
                {
                    yPlusLast = yPlus;

                    // The non-dimensional roughness height.
                    scalar KsPlus = yPlus*dKsPlusdYPlus;

                    // The extra term in the law-of-the-wall.
                    scalar G = 0.0;

                    scalar yPlusGPrime = 0.0;

                    if (KsPlus >= 90)
                    {
                        const scalar t_1 = 1 + roughnessConstant_*KsPlus;
                        G = log(t_1);
                        yPlusGPrime = roughnessConstant_*KsPlus/t_1;
                    }
                    else if (KsPlus > 2.25)
                    {
                        const scalar t_1 = c_1*KsPlus - c_2;
                        const scalar t_2 = c_3*log(KsPlus) - c_4;
                        const scalar sint_2 = sin(t_2);
                        const scalar logt_1 = log(t_1);
                        G = logt_1*sint_2;
                        yPlusGPrime =
                            (c_1*sint_2*KsPlus/t_1) + (c_3*logt_1*cos(t_2));
                    }

                    scalar denom = 1.0 + log(E* yPlus) - G - yPlusGPrime;
                    if(mag(denom) > VSMALL)
                    {
                        yPlus = (kappaRe + yPlus*(1 - yPlusGPrime))/denom;
                        if( yPlus < 0 )
                        {
                            yPlus = 0;
                        }
                    }
                    else
                    {
                        // Ensure immediate end and mutw = 0.
                        yPlus = 0;
                    }

                } while
                (
                    mag(ryPlusLam*(yPlus - yPlusLast)) > 0.0001
                 && ++iter < 10
                 && yPlus > VSMALL
                );

                if (yPlus > yPlusLam)
                {
                    mutw[facei] = muw[facei]*(yPlus*yPlus/Re - 1);
                }
                else
                {
                    mutw[facei] = 0.0;
                }
            }
        }
    }
    else
    {
        // Smooth Walls.
        forAll(mutw, facei)
        {
            const scalar magUpara = magUp[facei];
            const scalar Re = rho[facei]*magUpara/(muw[facei]*ry[facei]);
            const scalar kappaRe = kappa*Re;

            scalar yPlus = yPlusLam;
            const scalar ryPlusLam = 1.0/yPlus;

            int iter = 0;
            scalar yPlusLast = 0.0;

            do
            {
                yPlusLast = yPlus;
                yPlus = (kappaRe + yPlus)/(1.0 + log(E*yPlus));

            } while
            (
                mag(ryPlusLam*(yPlus - yPlusLast)) > 0.0001
             && ++iter < 10
            );

            if (yPlus > yPlusLam)
            {
                mutw[facei] = muw[facei]*(yPlus*yPlus/Re - 1);
            }
            else
            {
                mutw[facei] = 0.0;
            }
        }
    }
}


void mutStandardRoughWallFunctionFvPatchScalarField::write(Ostream& os) const
{
    fixedValueFvPatchScalarField::write(os);
    os.writeKeyword("roughnessHeight")
        << roughnessHeight_ << token::END_STATEMENT << nl;
    os.writeKeyword("roughnessConstant")
        << roughnessConstant_ << token::END_STATEMENT << nl;
    os.writeKeyword("roughnessFudgeFactor")
        << roughnessFudgeFactor_ << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    mutStandardRoughWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace compressible
} // End namespace Foam

// ************************************************************************* //
