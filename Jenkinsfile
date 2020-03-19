pipeline {
    agent any
    options {
      buildDiscarder(logRotator(daysToKeepStr: '90', artifactDaysToKeepStr: '90'))
    }
    
    post {
      failure {
        script {
           currentBuild.result = 'FAILED'
        }
      }
      success {
         script {
           currentBuild.result = 'SUCCESS'
        }
      }
    }
    
    stages {
        stage('Build') {
            parallel {
                stage('UnitTest') {
                    steps {
						bat 'ceedling.cmd gcov:all'
                        bat 'ceedling.cmd utils:gcov'
                        xunit([Custom(customXSL: 'ceedling/unity.xsl', deleteOutputFiles: true, failIfNotNew: true, pattern: 'build/artifacts/gcov/report.xml', skipNoTestFiles: false, stopProcessingIfError: true)])
                        cobertura coberturaReportFile: 'build/artifacts/gcov/GcovCoverageResults.xml'
                    }
                }
                stage('CppCheck') {
                    steps {
                        bat '"C:/Program Files/Cppcheck/cppcheck.exe" -j4 --enable=all -I./src/ --xml --xml-version=2 ./src 2> cppcheck.xml'
                        publishCppcheck pattern:'cppcheck.xml', failureThreshold:'2'
                    }
                }
            }
        }
    }
}
