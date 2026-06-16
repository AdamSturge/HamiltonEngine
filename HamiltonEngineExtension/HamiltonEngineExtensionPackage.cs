using System;
using System.CodeDom.Compiler;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Threading;
using Microsoft.VisualStudio.Debugger;
using Microsoft.VisualStudio.Debugger.ComponentInterfaces;
using Microsoft.VisualStudio.Debugger.Evaluation;
using Microsoft.VisualStudio.Debugger.Evaluation.IL;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Task = System.Threading.Tasks.Task;

namespace HamiltonEngineExtension
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [Guid(HamiltonEngineExtensionPackage.PackageGuidString)]
    public sealed class HamiltonEngineExtensionPackage : AsyncPackage
    {
        /// <summary>
        /// HamiltonEngineExtensionPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "6d7509d1-2dde-4403-b496-71dd04620acd";

        #region Package Members

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            // When initialized asynchronously, the current thread may be a background thread at this point.
            // Do any initialization that requires the UI thread after switching to the UI thread.
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);
        }

        #endregion
    }

    
    public class HamiltonCos : IDkmIntrinsicFunctionEvaluator140
    {
       public DkmILEvaluationResult[] Execute(DkmILExecuteIntrinsic executeIntrinsic, DkmILContext iLContext, 
            DkmCompiledILInspectionQuery inspectionQuery, DkmILEvaluationResult[] arguments, 
            ReadOnlyCollection<DkmCompiledInspectionQuery> subroutines, 
            out DkmILFailureReason failureReason)
        {
            // 1. Get information about the calling context
            uint functionId = executeIntrinsic.Id; 
            Guid sourceId = executeIntrinsic.SourceId; 

            DkmILEvaluationResult[] Results = new DkmILEvaluationResult[1];
            if (functionId == 0)
            {
                if (arguments.Length != 1)
                {
                    failureReason = DkmILFailureReason.StringTooLong;
                }
                else
                {
                    byte[] InBytes = new byte[4];
                    arguments[0].ResultBytes.CopyTo(InBytes, 0);
                    float Theta = System.BitConverter.ToSingle(InBytes, 0);

                    Theta += 1.0f;

                    byte[] OutBytes = System.BitConverter.GetBytes(Theta);
                    ReadOnlyCollection<byte> OutBytesReadOnly = new ReadOnlyCollection<byte>(OutBytes);

                    failureReason = DkmILFailureReason.None;
                    Results[0] = DkmILEvaluationResult.Create(sourceId, OutBytesReadOnly);
                }
            }
            else 
            {
                failureReason = DkmILFailureReason.DivideByZero;
            }

            return Results;
        }

    }
}
